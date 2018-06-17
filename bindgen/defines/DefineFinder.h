#ifndef SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
#define SCALA_NATIVE_BINDGEN_DEFINEFINDER_H

#include "../ir/IR.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>

class DefineFinder : public clang::PPCallbacks {
  public:
    DefineFinder(IR &ir, const clang::CompilerInstance &compiler,
                 clang::Preprocessor &pp);

    void MacroDefined(const clang::Token &MacroNameTok,
                      const clang::MacroDirective *MD) override;

    void MacroUndefined(const clang::Token &MacroNameTok,
                        const clang::MacroDefinition &MD);

  private:
    IR &ir;
    const clang::CompilerInstance &compiler;
    clang::Preprocessor &pp;

    const clang::Token *getFinalIdentifier(const clang::Token &token) const;

    void addNumericConstantDefine(const std::string &macroName,
                                  std::string literal,
                                  const clang::Token *finalToken);

    void getTypeOfIntLiteralWithoutEnding(clang::NumericLiteralParser parser,
                                          std::string &literal,
                                          std::string &type);
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
