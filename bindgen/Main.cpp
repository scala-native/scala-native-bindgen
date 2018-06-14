#include "visitor/ScalaFrontendActionFactory.h"
#include <clang/Tooling/CommonOptionsParser.h>

int main(int argc, char *argv[]) {
    llvm::cl::OptionCategory Category("Binding Generator");
    llvm::cl::extrahelp CommonHelp(
        clang::tooling::CommonOptionsParser::HelpMessage);
    llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please "
                                 "specify lib name with parameter name\n");

    llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category),
                                       llvm::cl::desc("Library name"));
    llvm::cl::opt<std::string> StdHeaders(
        "std-headers", llvm::cl::cat(Category),
        llvm::cl::desc("Path to a file with the list of headers for which "
                       "bindings\n"
                       "will not be generated. "
                       "The list contains header files names\n"
                       "and package names that contain bindings for these "
                       "headers.\nExample:\n"
                       "math.h=scala.scalanative.native.math\n"
                       "stdlib.h=scala.scalanative.native.stdlib"));
    llvm::cl::opt<bool> PrintHeadersLocation(
        "location", llvm::cl::cat(Category),
        llvm::cl::desc("Print list of parsed headers"));
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
    clang::tooling::CommonOptionsParser op(argc, (const char **)argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(),
                                   op.getSourcePathList());

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

    auto stdhead = StdHeaders.getValue();
    if (!stdhead.empty()) {
        headerMan.LoadConfig(stdhead);
    }

    locations.clear();

    IR ir(libName, linkName, objectName, Package.getValue());
    ScalaFrontendActionFactory actionFactory(ir);

    int result = Tool.run(&actionFactory);

    auto printLoc = PrintHeadersLocation.getValue();
    if (printLoc) {
        for (const auto &location : locations) {
            llvm::outs() << location.c_str();
        }
    } else {
        ir.generate(ExcludePrefix.getValue());
        llvm::outs() << ir;
    }
    llvm::outs().flush();
    return result;
}
