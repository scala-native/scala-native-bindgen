#ifndef SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTION_H
#define SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTION_H

#include "TreeConsumer.h"

#include "../ir/IR.h"
#include <clang/Frontend/FrontendActions.h>

/**
 * Creates ASTConsumer which will go through all top-level
 * declarations and execute visitor on some of them.
 */
class ScalaFrontendAction : public clang::ASTFrontendAction {
  public:
    explicit ScalaFrontendAction(IR &ir);

    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &CI,
                      clang::StringRef file) override;

  private:
    IR &ir;
};

#endif // SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTION_H
