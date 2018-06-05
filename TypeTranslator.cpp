#include "TypeTranslator.h"
#include "Utils.h"

TypeTranslator::TypeTranslator(clang::ASTContext *ctx_) : ctx(ctx_), typeMap() {

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
    typeMap["void*"] = "native.Ptr[Byte]";
}

std::string
TypeTranslator::TranslateFunctionPointer(const clang::QualType &qtpe,
                                         const std::string *avoid) {
    const clang::PointerType *ptr =
        qtpe.getTypePtr()->getAs<clang::PointerType>();
    const clang::QualType &inner = ptr->getPointeeType();

    if (inner->isFunctionProtoType()) {
        const clang::FunctionProtoType *fc =
            inner->getAs<clang::FunctionProtoType>();
        std::string ret = Translate(fc->getReturnType(), avoid);
        std::string params = "";
        int counter = 0;

        for (const clang::QualType &param : fc->param_types()) {
            params += Translate(param, avoid);
            params += ", ";
            counter++;
        }

        std::string variad = "";

        if (fc->isVariadic()) {
            counter++;
            variad = "native.CVararg, ";
        }

        return std::string("native.CFunctionPtr") + std::to_string(counter) +
               "[" + params + variad + ret + "]";

    } else {
        llvm::errs() << "Unsupported function pointer type: "
                     << qtpe.getAsString() << "\n";
        llvm::errs().flush();
        exit(-1);
    }
}

std::string TypeTranslator::TranslatePointer(const clang::QualType &pte,
                                             const std::string *avoid) {

    if (pte->isBuiltinType()) {
        const clang::BuiltinType *as = pte->getAs<clang::BuiltinType>();

        // Take care of void*
        if (as->getKind() == clang::BuiltinType::Void) {
            return "native.Ptr[Byte]";
        }

        // Take care of char*
        if (as->getKind() == clang::BuiltinType::Char_S ||
            as->getKind() == clang::BuiltinType::SChar) {
            return "native.CString";
        }
    }

    return std::string("native.Ptr[") + Translate(pte, avoid) +
           std::string("]");
}

std::string
TypeTranslator::TranslateStructOrUnion(const clang::QualType &qtpe) {
    if (qtpe->hasUnnamedOrLocalType()) {
        // TODO: Verify that the local part is not a problem
        uint64_t size = ctx->getTypeSize(qtpe);
        return "native.CArray[Byte, " + uint64ToScalaNat(size) + "]";
    }

    std::string name = qtpe.getUnqualifiedType().getAsString();

    // TODO: do it properly
    size_t f = name.find(std::string("struct __dirstream"));
    if (f != std::string::npos) {
        return std::string("native.CArray[Byte, Digit[_3, Digit[_2, _0]]]");
    }

    f = name.find(" ");
    if (f != std::string::npos) {
        return name.replace(f, std::string(" ").length(), "_");
    }
    return name;
}

std::string TypeTranslator::TranslateEnum(const clang::QualType &qtpe) {
    std::string name = qtpe.getUnqualifiedType().getAsString();
    size_t f = name.find(" ");
    if (f != std::string::npos) {
        return name.replace(f, std::string(" ").length(), "_");
    }
    return name;
}

std::string
TypeTranslator::TranslateConstantArray(const clang::ConstantArrayType *ar,
                                       const std::string *avoid) {
    const uint64_t size = ar->getSize().getZExtValue();
    const std::string nat = uint64ToScalaNat(size);
    return "native.CArray[" + Translate(ar->getElementType(), avoid) + ", " +
           nat + "]";
}

std::string TypeTranslator::Translate(const clang::QualType &qtpe,
                                      const std::string *avoid) {

    // Warning / Sanity checks

    if (qtpe.isConstQualified() || (ctx && qtpe.isConstant(*ctx))) {
        llvm::errs() << "Warning: Const qualifier not supported\n";
        llvm::errs().flush();
    }
    if (qtpe.isVolatileQualified()) {
        llvm::errs() << "Warning: Volatile qualifier not supported\n";
        llvm::errs().flush();
    }
    if (qtpe.isRestrictQualified()) {
        llvm::errs() << "Warning: Restrict qualifier not supported\n";
        llvm::errs().flush();
    }

    const clang::Type *tpe = qtpe.getTypePtr();

    if (typeEquals(tpe, avoid)) {
        // This is a type that we want to avoid the usage.
        //Êxample: A struct that has a pointer to itself
        uint64_t size = ctx->getTypeSize(tpe);
        return "native.CArray[Byte, " + uint64ToScalaNat(size) + "]";
    }

    if (tpe->isFunctionPointerType()) {
        return TranslateFunctionPointer(qtpe, avoid);

    } else if (tpe->isPointerType()) {
        return TranslatePointer(
            tpe->getAs<clang::PointerType>()->getPointeeType(), avoid);

    } else if (qtpe->isStructureType() || qtpe->isUnionType()) {
        return handleReservedWords(TranslateStructOrUnion(qtpe));

    } else if (qtpe->isEnumeralType()) {
        return TranslateEnum(qtpe);

    } else if (qtpe->isConstantArrayType()) {
        return TranslateConstantArray(ctx->getAsConstantArrayType(qtpe), avoid);
    } else if (qtpe->isArrayType()) {
        return TranslatePointer(ctx->getAsArrayType(qtpe)->getElementType(),
                                avoid);
    } else {

        auto found = typeMap.find(qtpe.getUnqualifiedType().getAsString());
        if (found != typeMap.end()) {
            return handleReservedWords(found->second);
        } else {
            // TODO: Properly handle non-default types
            return handleReservedWords(qtpe.getUnqualifiedType().getAsString());
        }
    }
}

void TypeTranslator::AddTranslation(std::string from, std::string to) {
    typeMap[from] = to;
}
