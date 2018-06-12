#ifndef SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
#define SCALA_NATIVE_BINDGEN_DEFINEFINDER_H

#include "../ir/IR.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/PPCallbacks.h>

class DefineFinder : public clang::PPCallbacks {
  public:
    DefineFinder(IR &ir, const clang::CompilerInstance &compiler);

    void MacroDefined(const clang::Token &MacroNameTok,
                      const clang::MacroDirective *MD) override;

  private:
    IR &ir;
    const clang::CompilerInstance &compiler;
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
