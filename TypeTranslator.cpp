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

std::string TypeTranslator::Translate(const clang::QualType& tpe){
	auto found = typeMap.find(tpe.getAsString());
	if(found != typeMap.end()){
		return found->second;
	} else {
		//TODO: Properly handle non-default types
		return tpe.getAsString();
	} 
}

void TypeTranslator::AddTranslation(std::string from, std::string to){
	typeMap[from] = to;
}