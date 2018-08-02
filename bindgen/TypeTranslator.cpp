#include "TypeTranslator.h"
#include "Utils.h"
#include "ir/types/FunctionPointerType.h"
#include "clang/AST/RecordLayout.h"

TypeTranslator::TypeTranslator(clang::ASTContext *ctx_, IR &ir)
    : ctx(ctx_), ir(ir), typeMap() {

    // Native Types
    typeMap["void"] = "Unit";
    typeMap["bool"] = "native.CBool";
    typeMap["_Bool"] = "native.CBool";
    typeMap["char"] = "native.CChar";
    typeMap["signed char"] = "native.CSignedChar";
    typeMap["unsigned char"] = "native.CUnsignedChar";
    typeMap["short"] = "native.CShort";
    typeMap["unsigned short"] = "native.CUnsignedShort";
    typeMap["int"] = "native.CInt";
    typeMap["long int"] = "native.CLongInt";
    typeMap["unsigned int"] = "native.CUnsignedInt";
    typeMap["unsigned long int"] = "native.CUnsignedLongInt";
    typeMap["long"] = "native.CLong";
    typeMap["unsigned long"] = "native.CUnsignedLong";
    typeMap["long long"] = "native.CLongLong";
    typeMap["unsigned long long"] = "native.CUnsignedLongLong";
    typeMap["size_t"] = "native.CSize";
    typeMap["ptrdiff_t"] = "native.CPtrDiff";
    typeMap["wchar_t"] = "native.CWideChar";
    typeMap["char16_t"] = "native.CChar16";
    typeMap["char32_t"] = "native.CChar32";
    typeMap["float"] = "native.CFloat";
    typeMap["double"] = "native.CDouble";
    typeMap["long double"] = "native.CDouble";
}

std::shared_ptr<Type>
TypeTranslator::translateFunctionPointer(const clang::QualType &qtpe) {
    const auto *ptr = qtpe.getTypePtr()->getAs<clang::PointerType>();
    const clang::QualType &inner = ptr->getPointeeType();

    if (inner->isFunctionProtoType()) {
        const auto *fc = inner->getAs<clang::FunctionProtoType>();
        std::shared_ptr<Type> returnType = translate(fc->getReturnType());
        std::vector<std::shared_ptr<const Type>> parametersTypes;

        for (const clang::QualType &param : fc->param_types()) {
            parametersTypes.push_back(translate(param));
        }

        return std::make_shared<FunctionPointerType>(
            returnType, parametersTypes, fc->isVariadic());

    } else {
        llvm::errs() << "Unsupported function pointer type: "
                     << qtpe.getAsString() << "\n";
        llvm::errs().flush();
        exit(-1);
    }
}

std::shared_ptr<Type>
TypeTranslator::translatePointer(const clang::QualType &pte) {

    if (pte->isBuiltinType()) {
        const clang::BuiltinType *as = pte->getAs<clang::BuiltinType>();

        // Take care of void*
        if (as->getKind() == clang::BuiltinType::Void) {
            return std::make_shared<PointerType>(
                std::make_shared<PrimitiveType>("Byte"));
        }

        // Take care of char*
        if (as->getKind() == clang::BuiltinType::Char_S ||
            as->getKind() == clang::BuiltinType::SChar) {
            // TODO: new PointerType(new PrimitiveType("native.CChar"))
            return std::make_shared<PrimitiveType>("native.CString");
        }
    }

    return std::make_shared<PointerType>(translate(pte));
}

std::shared_ptr<Type>
TypeTranslator::translateStructOrUnionOrEnum(const clang::QualType &qtpe) {
    std::string name = qtpe.getUnqualifiedType().getAsString();
    std::string nameWithoutSpace = replaceChar(name, " ", "_");

    /* If the struct was already declared then there is a TypeDef instance
     * with appropriate name.
     *
     * If there is no such TypeDef then the type is opaque and TypeDef with
     * nullptr will be generated for the type. */

    std::shared_ptr<TypeDef> typeDef = ir.getTypeDefWithName(nameWithoutSpace);
    if (typeDef) {
        return typeDef;
    }
    /* type is not yet defined.
     * TypeDef with nullptr will be created.
     * nullptr will be replaced by actual type when the type is declared. */
    typeDef = ir.addTypeDef(nameWithoutSpace, nullptr, nullptr);
    return typeDef;
}

std::shared_ptr<Type>
TypeTranslator::translateStructOrUnion(const clang::QualType &qtpe) {
    if (qtpe->hasUnnamedOrLocalType()) {
        if (qtpe->isStructureType()) {
            std::string name =
                "anonymous_" + std::to_string(anonymousStructId++);
            clang::RecordDecl *record = qtpe->getAsStructureType()->getDecl();
            return addStructDefinition(record, name);
        } else if (qtpe->isUnionType()) {
            std::string name =
                "anonymous_" + std::to_string(anonymousUnionId++);
            clang::RecordDecl *record = qtpe->getAsUnionType()->getDecl();
            return addUnionDefinition(record, name);
        }
        return nullptr;
    }
    return translateStructOrUnionOrEnum(qtpe);
}

std::shared_ptr<Type>
TypeTranslator::translateConstantArray(const clang::ConstantArrayType *ar) {
    const uint64_t size = ar->getSize().getZExtValue();
    std::shared_ptr<Type> elementType = translate(ar->getElementType());
    if (elementType == nullptr) {
        llvm::errs() << "Failed to translate array type "
                     << ar->getElementType().getAsString() << "\n";
        elementType = std::make_shared<PrimitiveType>("Byte");
    }

    return std::make_shared<ArrayType>(elementType, size);
}

std::shared_ptr<Type> TypeTranslator::translate(const clang::QualType &qtpe) {

    const clang::Type *tpe = qtpe.getTypePtr();

    if (tpe->isFunctionType()) {
        return nullptr;
    }

    if (tpe->isFunctionPointerType()) {
        return translateFunctionPointer(qtpe);

    } else if (tpe->isPointerType()) {
        return translatePointer(
            tpe->getAs<clang::PointerType>()->getPointeeType());

    } else if (qtpe->isStructureType() || qtpe->isUnionType()) {
        return translateStructOrUnion(qtpe);

    } else if (qtpe->isEnumeralType()) {
        return translateEnum(qtpe);

    } else if (qtpe->isConstantArrayType()) {
        return translateConstantArray(ctx->getAsConstantArrayType(qtpe));
    } else if (qtpe->isArrayType()) {
        return translatePointer(ctx->getAsArrayType(qtpe)->getElementType());
    } else {

        auto found = typeMap.find(qtpe.getUnqualifiedType().getAsString());
        if (found != typeMap.end()) {
            return std::make_shared<PrimitiveType>(found->second);
        } else {
            return ir.getTypeDefWithName(
                qtpe.getUnqualifiedType().getAsString());
        }
    }
}

std::string TypeTranslator::getTypeFromTypeMap(std::string cType) {
    auto it = typeMap.find(cType);
    if (it != typeMap.end()) {
        return (*it).second;
    }
    return "";
}

std::shared_ptr<Location> TypeTranslator::getLocation(clang::Decl *decl) {
    clang::SourceManager &sm = ctx->getSourceManager();
    std::string filename = std::string(sm.getFilename(decl->getLocation()));
    std::string path = getRealPath(filename.c_str());

    unsigned lineNumber = sm.getSpellingLineNumber(decl->getLocation());
    return std::make_shared<Location>(path, lineNumber);
}

std::shared_ptr<TypeDef>
TypeTranslator::addUnionDefinition(clang::RecordDecl *record,
                                   std::string name) {
    std::vector<std::shared_ptr<Field>> fields;

    for (const clang::FieldDecl *field : record->fields()) {
        std::string fname = field->getNameAsString();
        std::shared_ptr<Type> ftype = translate(field->getType());

        fields.push_back(std::make_shared<Field>(fname, ftype));
    }

    uint64_t sizeInBits = ctx->getTypeSize(record->getTypeForDecl());
    assert(sizeInBits % 8 == 0);

    return ir.addUnion(name, std::move(fields), sizeInBits / 8,
                       getLocation(record));
}

std::shared_ptr<TypeDef>
TypeTranslator::addStructDefinition(clang::RecordDecl *record,
                                    std::string name) {
    std::string newName = "struct_" + name;

    if (record->hasAttr<clang::PackedAttr>()) {
        llvm::errs() << "Warning: struct " << name << " is packed. "
                     << "Packed structs are not supported by Scala Native. "
                     << "Access to fields will not work correctly.\n";
        llvm::errs().flush();
    }

    std::vector<std::shared_ptr<Field>> fields;
    const clang::ASTRecordLayout &recordLayout =
        ctx->getASTRecordLayout(record);

    bool isBitFieldStruct = false;
    for (const clang::FieldDecl *field : record->fields()) {
        if (field->isBitField()) {
            isBitFieldStruct = true;
        }
        std::shared_ptr<Type> ftype = translate(field->getType());
        uint64_t recordOffsetInBits =
            recordLayout.getFieldOffset(field->getFieldIndex());
        fields.push_back(std::make_shared<Field>(field->getNameAsString(),
                                                 ftype, recordOffsetInBits));
    }

    uint64_t sizeInBits = ctx->getTypeSize(record->getTypeForDecl());
    assert(sizeInBits % 8 == 0);

    return ir.addStruct(name, std::move(fields), sizeInBits / 8,
                        getLocation(record),
                        record->hasAttr<clang::PackedAttr>(), isBitFieldStruct);
}

std::shared_ptr<Type>
TypeTranslator::translateEnum(const clang::QualType &type) {
    if (type->hasUnnamedOrLocalType()) {
        clang::EnumDecl *enumDecl = type->getAs<clang::EnumType>()->getDecl();
        return std::make_shared<PrimitiveType>(getTypeFromTypeMap(
            enumDecl->getIntegerType().getUnqualifiedType().getAsString()));
    }
    return translateStructOrUnionOrEnum(type);
}
