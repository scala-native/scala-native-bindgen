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
	{"unsigned char", "native.CUnsignedChar[1]"},
	{"short", "native.CShort"},
	{"unsigned short", "native.CUnsignedShort[1]"},
	{"int", "native.CInt"},
	{"long int", "native.CLongInt"},
	{"unsigned int", "native.CUnsignedInt[1]"},
	{"unsigned long int", "native.CUnsignedLongInt[1]"},
	{"long", "native.CLong"},
	{"unsigned long", "native.CUnsignedLong[1]"},
	{"long long", "native.CLongLong"},
	{"unsigned long long", "native.CUnsignedLongLong[1]"},
	{"size_t", "native.CSize"},
	{"ptrdiff_t", "native.CPtrDiff[2]"},
	{"wchar_t", "native.CWideChar"},
	{"char16_t", "native.CChar16"},
	{"char32_t", "native.CChar32"},
	{"float", "native.CFloat"},
	{"double", "native.CDouble"},
	{"void*", "native.Ptr[Byte][2]"},
	{"int*", "native.Ptr[native.CInt][2]"},
	{"char*", "native.CString[2][3]"}
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
    		params += TranslateType(parm->getType().getAsString());
    		params += " ";
    		params += parm->getNameAsString();
    		params += ", ";
    	}

    	//remove last ,
    	if(params != ""){
    		params = params.substr(0, params.size()-2);
    	}
      	
        llvm::outs() << "\t" << retType << " " << funcName << "(" << params << ")\n";
        return true;
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