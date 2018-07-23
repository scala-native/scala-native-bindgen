#include "TypeTranslator.h"
#include "Utils.h"
#include "ir/types/FunctionPointerType.h"
#include "ir/types/PointerType.h"

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
TypeTranslator::translateFunctionPointer(const clang::QualType &qtpe,
                                         const std::string *avoid) {
    const auto *ptr = qtpe.getTypePtr()->getAs<clang::PointerType>();
    const clang::QualType &inner = ptr->getPointeeType();

    if (inner->isFunctionProtoType()) {
        const auto *fc = inner->getAs<clang::FunctionProtoType>();
        std::shared_ptr<Type> returnType =
            translate(fc->getReturnType(), avoid);
        std::vector<std::shared_ptr<const Type>> parametersTypes;

        for (const clang::QualType &param : fc->param_types()) {
            parametersTypes.push_back(translate(param, avoid));
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
TypeTranslator::translatePointer(const clang::QualType &pte,
                                 const std::string *avoid) {

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

    return std::make_shared<PointerType>(translate(pte, avoid));
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
        // TODO: Verify that the local part is not a problem
        uint64_t sizeInBits = ctx->getTypeSize(qtpe);
        assert(sizeInBits % 8 == 0);
        return std::make_shared<ArrayType>(
            std::make_shared<PrimitiveType>("Byte"), sizeInBits / 8);
    }

    return translateStructOrUnionOrEnum(qtpe);
}

std::shared_ptr<Type>
TypeTranslator::translateConstantArray(const clang::ConstantArrayType *ar,
                                       const std::string *avoid) {
    const uint64_t size = ar->getSize().getZExtValue();
    std::shared_ptr<Type> elementType = translate(ar->getElementType(), avoid);
    if (elementType == nullptr) {
        llvm::errs() << "Failed to translate array type "
                     << ar->getElementType().getAsString() << "\n";
        elementType = std::make_shared<PrimitiveType>("Byte");
    }

    return std::make_shared<ArrayType>(elementType, size);
}

std::shared_ptr<Type> TypeTranslator::translate(const clang::QualType &qtpe,
                                                const std::string *avoid) {

    const clang::Type *tpe = qtpe.getTypePtr();

    if (typeEquals(tpe, avoid)) {
        // This is a type that we want to avoid the usage.
        // Êxample: A struct that has a pointer to itself
        uint64_t sizeInBits = ctx->getTypeSize(tpe);
        assert(sizeInBits % 8 == 0);
        return std::make_shared<ArrayType>(
            std::make_shared<PrimitiveType>("Byte"), sizeInBits / 8);
    }

    if (tpe->isFunctionType()) {
        return nullptr;
    }

    if (tpe->isFunctionPointerType()) {
        return translateFunctionPointer(qtpe, avoid);

    } else if (tpe->isPointerType()) {
        return translatePointer(
            tpe->getAs<clang::PointerType>()->getPointeeType(), avoid);

    } else if (qtpe->isStructureType()) {
        return translateStructOrUnion(qtpe);

    } else if (qtpe->isUnionType()) {
        return translateStructOrUnion(qtpe);

    } else if (qtpe->isEnumeralType()) {
        return translateStructOrUnionOrEnum(qtpe);

    } else if (qtpe->isConstantArrayType()) {
        return translateConstantArray(ctx->getAsConstantArrayType(qtpe), avoid);
    } else if (qtpe->isArrayType()) {
        return translatePointer(ctx->getAsArrayType(qtpe)->getElementType(),
                                avoid);
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
