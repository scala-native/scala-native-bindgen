#include "TypeTranslator.h"
#include <error.h>

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
}

std::string TypeTranslator::Translate(const clang::QualType& qtpe){

    //Warning / Sanity checks

    if(qtpe.isConstQualified() || (ctx && qtpe.isConstant(*ctx))){
        llvm::errs() << "Warning: Const qualifier not supported\n";
    }
    if(qtpe.isVolatileQualified()){
        llvm::errs() << "Warning: Volatile qualifier not supported\n";
    }
    if(qtpe.isRestrictQualified()){
        llvm::errs() << "Warning: Restrict qualifier not supported\n";
    }

    const clang::Type* tpe = qtpe.getTypePtr();

    if(tpe->isFunctionPointerType()){
        const clang::PointerType* ptr = tpe->getAs<clang::PointerType>();
        const clang::QualType& inner = ptr->getPointeeType();

        if(inner->isFunctionProtoType()) {
            const clang::FunctionProtoType* fc = inner->getAs<clang::FunctionProtoType>();
            std::string ret = Translate(fc->getReturnType());
            std::string params = "";
            int counter = 0;

            for(const clang::QualType& param: fc->param_types()){
                params += Translate(param);
                params += ",";
                counter++;
            }

            if(params != ""){
                //remove last ,
                params = params.substr(0, params.size()-1);

                return std::string("native.CFunctionPtr") + std::to_string(counter) + "[" + ret + "," + params + "]";
            } else{
                return std::string("native.CFunctionPtr") + std::to_string(counter) + "[" + ret + "]";
            }
        } else {
            llvm::errs() << "Unsupported function pointer type: " << qtpe.getAsString() << "\n";
            exit(-1);
        }

    } else if(tpe->isPointerType()){
        const clang::PointerType* ptr = tpe->getAs<clang::PointerType>();
        const clang::QualType& pte = ptr->getPointeeType();

        //Take care of void*
        if(pte->isBuiltinType()){
            const clang::BuiltinType* as = pte->getAs<clang::BuiltinType>();
            if(as->getKind() == clang::BuiltinType::Void){
               return "native.Ptr[Byte]";
            }
         }

        return std::string("native.Ptr[") + Translate(pte) + std::string("]");

    } else if(qtpe->isStructureType()){
        std::string name = qtpe.getAsString();
        size_t f = name.find(" ");
        if(f != std::string::npos){
            return name.replace(f, std::string(" ").length(), "_");
        }
        return name;
    } else {

        auto found = typeMap.find(qtpe.getUnqualifiedType().getAsString());
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
