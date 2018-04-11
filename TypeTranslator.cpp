#include "TypeTranslator.h"

TypeTranslator::TypeTranslator(clang::ASTContext* ctx_) : ctx(ctx_), typeMap() {

	//Native Types
	typeMap["void"] = "Unit";
	typeMap["bool"] = "native.CBool";
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
	typeMap["int*"] = "native.Ptr[native.CInt]";
	typeMap["char*"] = "native.CString";

}

std::string TypeTranslator::Translate(const clang::QualType& qtpe){

    //Warning / Sanity checks
    clang::Qualifiers quals = qtpe.getQualifiers();

    if(quals.hasConst() || (ctx && qtpe.isConstant(*ctx))){
        llvm::errs() << "Warning: Const qualifier not supported\n";
    }
    if(quals.hasVolatile()){
        llvm::errs() << "Warning: Volatile qualifier not supported\n";
    }
    if(quals.hasRestrict()){
        llvm::errs() << "Warning: Restrict qualifier not supported\n";
    }

    const clang::Type* tpe = qtpe.getTypePtr();

    if(tpe->isPointerType()){
        //Is it a pointer

        const clang::PointerType* ptr = tpe->getAs<clang::PointerType>();
        return std::string("native.Ptr[") + Translate(ptr->getPointeeType()) + std::string("]");
    } else {

        auto found = typeMap.find(tpe->getLocallyUnqualifiedSingleStepDesugaredType().getAsString());
        if(found != typeMap.end()){
            return found->second;
        } else {
            //TODO: Properly handle non-default types
            return qtpe.getAsString();
        }
    }



}

void TypeTranslator::AddTranslation(std::string from, std::string to){
	typeMap[from] = to;
}
