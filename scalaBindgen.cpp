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


class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
private:
    ASTContext *astContext;

public:
    explicit ExampleVisitor(CompilerInstance *CI) : astContext(&(CI->getASTContext())) {}

    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        std::string funcName = func->getNameInfo().getName().getAsString();
        std::cout << "\tvoid " << funcName << "()" << std::endl;
        return true;
    }

};



class ExampleASTConsumer : public clang::ASTConsumer {
private:
    ExampleVisitor *visitor;

public:
    explicit ExampleASTConsumer(CompilerInstance *CI) : visitor(new ExampleVisitor(CI)) {}

    // override this to call our ExampleVisitor on each top-level Decl
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
        return std::unique_ptr<clang::ASTConsumer>(new ExampleASTConsumer(&CI)); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {
    CommonOptionsParser op(argc, argv, Category);        
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());
       	
    std::cout << "@native.extern\nobject " << "example" << " {" << std::endl;
    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());
    std::cout << "}" << std::endl;

    return result;
}