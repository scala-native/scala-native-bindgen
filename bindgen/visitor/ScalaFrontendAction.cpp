#include "ScalaFrontendAction.h"

ScalaFrontendAction::ScalaFrontendAction(IR &ir,
                                         LocationManager &locationManager)
    : ir(ir), locationManager(locationManager) {}

std::unique_ptr<clang::ASTConsumer>
ScalaFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                       clang::StringRef file) {
    return std::unique_ptr<clang::ASTConsumer>(
        new TreeConsumer(&CI, ir, locationManager));
}
