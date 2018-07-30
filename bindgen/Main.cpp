#include "defines/DefineFinderActionFactory.h"
#include "ir/LocationManager.h"
#include "visitor/ScalaFrontendActionFactory.h"
#include <clang/Tooling/CommonOptionsParser.h>

int main(int argc, const char *argv[]) {
    llvm::cl::OptionCategory Category("Scala Native Binding Generator");
    llvm::cl::extrahelp CommonHelp(
        clang::tooling::CommonOptionsParser::HelpMessage);
    llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please "
                                 "specify lib name with parameter name\n");

    llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category),
                                       llvm::cl::desc("Library name"));

    llvm::cl::opt<std::string> ExcludePrefix(
        "exclude-prefix", llvm::cl::cat(Category),
        llvm::cl::desc("Functions and unused typedefs will be removed if their "
                       "names\nhave given prefix"));
    llvm::cl::opt<std::string> Package(
        "package", llvm::cl::cat(Category),
        llvm::cl::desc("Package name of generated Scala file"));
    llvm::cl::opt<bool> NoLinkName(
        "no-link", llvm::cl::cat(Category),
        llvm::cl::desc("Library does not require linking"));
    llvm::cl::opt<std::string> LinkName(
        "link", llvm::cl::cat(Category),
        llvm::cl::desc("Library to link with, e.g. -luv for libuv"));
    llvm::cl::opt<std::string> ReuseBindingsConfig(
        "binding-config", llvm::cl::cat(Category),
        llvm::cl::desc("Path to a config file that contains the information "
                       "about bindings that should be reused"));
    clang::tooling::CommonOptionsParser op(argc, argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(),
                                   op.getSourcePathList());

    if (op.getSourcePathList().size() != 1) {
        llvm::errs() << "Error: Only one file may be processed at a time.\n";
        llvm::errs().flush();
        return -1;
    }

    auto libName = LibName.getValue();
    if (libName.empty()) {
        llvm::errs()
            << "Error: Please specify the lib name using -name parameter\n";
        llvm::errs().flush();
        return -1;
    }

    auto linkName = LinkName.getValue();
    if (linkName.empty()) {
        linkName = libName;
    }
    if (NoLinkName.getValue()) {
        linkName = "";
    }

    auto objectName = libName;
    if (objectName == "native") {
        /* there are at most 3 objects in the file.
         * All of them will have distinct names. */
        objectName = "nativeLib";
    }

    std::string resolved = getRealPath(op.getSourcePathList()[0].c_str());
    LocationManager locationManager(resolved);

    auto reuseBindingsConfig = ReuseBindingsConfig.getValue();
    if (!reuseBindingsConfig.empty()) {
        locationManager.loadConfig(reuseBindingsConfig);
    }

    IR ir(libName, linkName, objectName, Package.getValue(), locationManager);

    DefineFinderActionFactory defineFinderActionFactory(ir);
    int result = Tool.run(&defineFinderActionFactory);
    if (result) {
        return result;
    }

    ScalaFrontendActionFactory actionFactory(ir);
    result = Tool.run(&actionFactory);

    ir.generate(ExcludePrefix.getValue());
    llvm::outs() << ir;
    llvm::outs().flush();
    return result;
}
