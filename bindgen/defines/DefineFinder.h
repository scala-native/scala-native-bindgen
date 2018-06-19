#ifndef SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
#define SCALA_NATIVE_BINDGEN_DEFINEFINDER_H

#include "../ir/IR.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/Preprocessor.h>

class DefineFinder : public clang::PPCallbacks {
  public:
    DefineFinder(IR &ir, const clang::CompilerInstance &compiler,
                 clang::Preprocessor &pp);

    void MacroDefined(const clang::Token &macroNameTok,
                      const clang::MacroDirective *md) override;

    void MacroUndefined(const clang::Token &macroNameTok,
                        const clang::MacroDefinition &md,
                        const clang::MacroDirective *undef) override;

  private:
    IR &ir;
    const clang::CompilerInstance &compiler;
    clang::Preprocessor &pp;

    void addNumericConstantDefine(const std::string &macroName,
                                  std::string literal,
                                  const clang::Token &token,
                                  bool positive = true);

    /**
     * Check if a number without `l` or `ll` ending fits into int or long
     * variable. Supports only non-negative numbers
     *
     * Updates `literal` and `type` parameters.
     */
    void getTypeOfIntLiteralWithoutEnding(clang::NumericLiteralParser parser,
                                          std::string &literal,
                                          std::string &type, bool positive);

    std::vector<clang::Token> *expandDefine(const clang::MacroDirective &md);

    bool isMacro(const clang::Token &token);

    bool isFunctionLikeMacro(const clang::Token &token);

    /**
     * @return true if number contained in parser fits into int type
     */
    template <typename signedT, typename unsignedT>
    bool fitsIntoType(clang::NumericLiteralParser parser, bool positive);
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
