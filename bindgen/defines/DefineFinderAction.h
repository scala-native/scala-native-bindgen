#ifndef SCALA_NATIVE_BINDGEN_DEFINEFINDERACTION_H
#define SCALA_NATIVE_BINDGEN_DEFINEFINDERACTION_H

#include "DefineFinder.h"
#include <clang/Frontend/FrontendActions.h>
#include <clang/Lex/Preprocessor.h>

class DefineFinderAction : public clang::PreprocessOnlyAction {
  public:
    explicit DefineFinderAction(IR &ir);

  protected:
    void ExecuteAction() override;

  private:
    IR &ir;
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDERACTION_H
