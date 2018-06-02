#include "visitor/ScalaFrontendActionFactory.h"
#include <clang/Tooling/CommonOptionsParser.h>


int main(int argc, char *argv[]) {
    llvm::cl::OptionCategory Category("Binding Generator");
    llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
    llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name with parameter name\n");

    llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category));
    llvm::cl::opt<std::string> StdHeaders("std-headers", llvm::cl::cat(Category));
    llvm::cl::opt<bool> PrintHeadersLocation("location", llvm::cl::cat(Category));
    llvm::cl::opt<std::string> ExcludePrefix("exclude-prefix", llvm::cl::cat(Category));
    
    clang::tooling::CommonOptionsParser op(argc, (const char**)argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto libName = LibName.getValue();
    if(libName.empty()){
        llvm::errs() << "Error: Please specify the lib name using -name parameter\n";
        llvm::errs().flush();
        return -1;
    }

    auto stdhead = StdHeaders.getValue();
    if(!stdhead.empty()){
        headerMan.LoadConfig(stdhead);
    }

    locations.clear();

    ScalaFrontendActionFactory actionFactory(libName);

    int result = Tool.run(&actionFactory);

    IR ir = actionFactory.getIntermediateRepresentation();

    auto printLoc = PrintHeadersLocation.getValue();
    if (printLoc) {
        for (const auto &location: locations) {
            llvm::outs() << location.c_str();
        }
    } else {
        ir.generate(ExcludePrefix.getValue());
        llvm::outs() << ir;
    }
    llvm::outs().flush();
    return result;
}
