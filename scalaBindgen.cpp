#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <iostream>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;


static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name");
static llvm::cl::opt<std::string> LibName("name", cl::cat(Category));

std::map<std::string, std::string> nativeTypes = {
	{"void", "Unit"},
	{"bool", "native.CBool"},
	{"char", "native.CChar"},
	{"signed char", "native.CSignedChar"},
	{"unsigned char", "native.CUnsignedChar"},
	{"short", "native.CShort"},
	{"unsigned short", "native.CUnsignedShort"},
	{"int", "native.CInt"},
	{"long int", "native.CLongInt"},
	{"unsigned int", "native.CUnsignedInt"},
	{"unsigned long int", "native.CUnsignedLongInt"},
	{"long", "native.CLong"},
	{"unsigned long", "native.CUnsignedLong"},
	{"long long", "native.CLongLong"},
	{"unsigned long long", "native.CUnsignedLongLong"},
	{"size_t", "native.CSize"},
	{"ptrdiff_t", "native.CPtrDiff"},
	{"wchar_t", "native.CWideChar"},
	{"char16_t", "native.CChar16"},
	{"char32_t", "native.CChar32"},
	{"float", "native.CFloat"},
	{"double", "native.CDouble"},
	{"void*", "native.Ptr[Byte]"},
	{"int*", "native.Ptr[native.CInt]"},
	{"char*", "native.CString"}
};

std::string TranslateType(std::string type){
	auto native = nativeTypes.find(type);
	if(native != nativeTypes.end()){
		return native -> second;
	} else {
		//TODO: Properly handle non-default types
		return type;
	}
}


class TreeVisitor : public RecursiveASTVisitor<TreeVisitor> {
private:
    ASTContext *astContext;

public:
    explicit TreeVisitor(CompilerInstance *CI) : astContext(&(CI->getASTContext())) {}

    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        std::string funcName = func->getNameInfo().getName().getAsString();
        std::string retType = TranslateType(func->getReturnType().getAsString());
        std::string params = "";

    	for (const auto *parm : func->parameters()){
    		//Handle default values
    		params += parm->getNameAsString();
    		params += ": ";
    		params += TranslateType(parm->getType().getAsString());
    		params += ", ";
    	}

    	//remove last ,
    	if(params != ""){
    		params = params.substr(0, params.size()-2);
    	}
      	
        llvm::outs() << "\tdef " << funcName << "(" << params << ") : " + retType + "\n";
        return true;
    }

	virtual bool VisitTypedefDecl(TypedefDecl *tpdef){    	
		//TODO: Understand difference between typedef and typedef-name
		std::string name = tpdef->getName();
		std::string tpe = TranslateType(tpdef->getUnderlyingType().getAsString());
    	llvm::outs() << "\ttype " << name << " = " << tpe << "\n";
    	return true;
    }

    virtual bool VisitEnumDecl(EnumDecl *enumdecl){
    	std::string enumName = enumdecl->getNameAsString();
    	llvm::outs() << "\ttype " << enumName << " = navtive.CInt\n";

    	int i = 0;
    	for (const EnumConstantDecl* en : enumdecl->enumerators()){
    		llvm::outs() << "\tval " << en->getNameAsString() << "__" << enumName << " = " << i++ << "\n"; 
    	}


    	return true;
    }

    virtual bool VisitRecordDecl (RecordDecl *record){
    	if(record->isUnion()){


      		return true;
    	} else if (record->isStruct()){
    		std::string structName = record->getNameAsString();
    		
    		llvm::outs() << "\ttype " << structName << " = " << "native.CStruct";

    		int counter = 0;
    		std::string fields = "";

    		for(const FieldDecl* field : record->fields()){
    			fields += TranslateType(field->getType().getAsString()) + ";";
    			counter++;
    		}

	    	//remove last ,
	    	if(fields != ""){
	    		fields = fields.substr(0, fields.size()-1);
	    	}
	      	
    		llvm::outs() << counter << "[" << fields << "]\n";

	    	return true;
    	}
    	return false;
    }

};



class TreeConsumer : public clang::ASTConsumer {
private:
    TreeVisitor *visitor;

public:
    explicit TreeConsumer(CompilerInstance *CI) : visitor(new TreeVisitor(CI)) {}

    // override this to call our TreeVisitor on each top-level Decl
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;    
            visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
    }

};



class ExampleFrontendAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return std::unique_ptr<clang::ASTConsumer>(new TreeConsumer(&CI)); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {
    CommonOptionsParser op(argc, argv, Category);        
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto lib = LibName.getValue();
    if(lib == ""){
    	llvm::errs() << "Error: Please specify the lib name using -name paramter\n";
    	return -1;
    }

    llvm::outs() << "@native.extern\nobject " << lib<< " {\n";
    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());
    llvm::outs() << "}\n";

    return result;
}