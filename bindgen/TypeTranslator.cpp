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
}

Type *TypeTranslator::translateFunctionPointer(const clang::QualType &qtpe,
                                               const std::string *avoid) {
    const auto *ptr = qtpe.getTypePtr()->getAs<clang::PointerType>();
    const clang::QualType &inner = ptr->getPointeeType();

    if (inner->isFunctionProtoType()) {
        const auto *fc = inner->getAs<clang::FunctionProtoType>();
        Type *returnType = translate(fc->getReturnType(), avoid);
        std::vector<Type *> parametersTypes;

        for (const clang::QualType &param : fc->param_types()) {
            parametersTypes.push_back(translate(param, avoid));
        }

        return new FunctionPointerType(returnType, parametersTypes,
                                       fc->isVariadic());

    } else {
        llvm::errs() << "Unsupported function pointer type: "
                     << qtpe.getAsString() << "\n";
        llvm::errs().flush();
        exit(-1);
    }
}

Type *TypeTranslator::translatePointer(const clang::QualType &pte,
                                       const std::string *avoid) {

    if (pte->isBuiltinType()) {
        const clang::BuiltinType *as = pte->getAs<clang::BuiltinType>();

        // Take care of void*
        if (as->getKind() == clang::BuiltinType::Void) {
            return new PointerType(new PrimitiveType("Byte"));
        }

        // Take care of char*
        if (as->getKind() == clang::BuiltinType::Char_S ||
            as->getKind() == clang::BuiltinType::SChar) {
            // TODO: new PointerType(new PrimitiveType("native.CChar"))
            return new PrimitiveType("native.CString");
        }
    }

    return new PointerType(translate(pte, avoid));
}

Type *
TypeTranslator::translateStructOrUnionOrEnum(const clang::QualType &qtpe) {
    std::string name = qtpe.getUnqualifiedType().getAsString();

    auto it = aliasesMap.find(name);
    if (it != aliasesMap.end()) {
        /* name contains space: struct <name>.
         * Use type alias instead struct type */
        return (*it).second;
    }
    /* type has typedef alias */
    return ir.getTypeDefWithName(name);
}

Type *TypeTranslator::translateStructOrUnion(const clang::QualType &qtpe) {
    if (qtpe->hasUnnamedOrLocalType()) {
        // TODO: Verify that the local part is not a problem
        uint64_t size = ctx->getTypeSize(qtpe);
        return new ArrayType(new PrimitiveType("Byte"), size);
    }

    return translateStructOrUnionOrEnum(qtpe);
}

Type *TypeTranslator::translateConstantArray(const clang::ConstantArrayType *ar,
                                             const std::string *avoid) {
    const uint64_t size = ar->getSize().getZExtValue();
    return new ArrayType(translate(ar->getElementType(), avoid), size);
}

Type *TypeTranslator::translate(const clang::QualType &qtpe,
                                const std::string *avoid) {

    const clang::Type *tpe = qtpe.getTypePtr();

    if (typeEquals(tpe, avoid)) {
        // This is a type that we want to avoid the usage.
        // Êxample: A struct that has a pointer to itself
        uint64_t size = ctx->getTypeSize(tpe);
        return new ArrayType(new PrimitiveType("Byte"), size);
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
            return new PrimitiveType(found->second);
        } else {
            return ir.getTypeDefWithName(
                qtpe.getUnqualifiedType().getAsString());
        }
    }
}

void TypeTranslator::addAlias(std::string cName, Type *type) {
    aliasesMap[cName] = type;
}

std::string TypeTranslator::getTypeFromTypeMap(std::string cType) {
    auto it = typeMap.find(cType);
    if (it != typeMap.end()) {
        return (*it).second;
    }
    return "";
}
