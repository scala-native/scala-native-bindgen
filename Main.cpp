#include "ScalaFrontend.h"
#include "Utils.h"


static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name\n");
static llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category));
static llvm::cl::opt<std::string> StdHeaders("stdHeaders", llvm::cl::cat(Category));
static llvm::cl::opt<bool> PrintHeadersLocation ("location", llvm::cl::cat(Category));


int main(int argc, char *argv[]) {
    clang::tooling::CommonOptionsParser op(argc, (const char**)argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto lib = LibName.getValue();
    if(lib == ""){
        llvm::errs() << "Error: Please specify the lib name using -name paramter\n";
        return -1;
    }

    auto stdhead = StdHeaders.getValue();
    if(stdhead != ""){
        headerMan.LoadConfig(stdhead);
    }

    declarations = "";
    enums = "";
    helpers = "";
    locations.clear();

    int result = Tool.run(clang::tooling::newFrontendActionFactory<ScalaFrontendAction>().get());

    auto printLoc = PrintHeadersLocation.getValue();

    if(printLoc){
        for(const auto& location: locations){
            llvm::outs() << location;
        }

    } else {
        if(declarations != "" || enums != "")
        llvm::outs() << "import scala.scalanative._\n"
                     << "import scala.scalanative.native._\n"
                     << "import scala.scalanative.native.Nat._\n\n";

        if(declarations != ""){
            llvm::outs() << "@native.link(\"" << lib << "\")\n"
                         << "@native.extern\n"
                         << "object " << lib << " {\n"
                         << declarations
                         << "}\n\n";
        }

        if(enums != "" || helpers != ""){
            llvm::outs() << "import " + lib + "._\n\n";
        }

        if(enums != ""){
            llvm::outs() << "object " << lib << "Enums {\n"
                         << enums
                         << "}\n\n";
        }

        if(helpers != ""){
            llvm::outs() << "object " << lib << "Helpers {\n"
                         << helpers
                         << "}\n\n";
        }
    }
    return result;
}
