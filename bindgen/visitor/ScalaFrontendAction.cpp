#include "ScalaFrontendAction.h"

ScalaFrontendAction::ScalaFrontendAction(IR *ir) : ir(ir) {}

std::unique_ptr<clang::ASTConsumer>
ScalaFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                       clang::StringRef file) {
    return std::unique_ptr<clang::ASTConsumer>(new TreeConsumer(&CI, ir));
}
