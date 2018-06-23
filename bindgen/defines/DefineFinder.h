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
     * Check if the number fits into int or long variable.
     *
     * @return type of the number
     */
    Type *getTypeOfIntegerLiteral(const clang::NumericLiteralParser &parser,
                                  const std::string &literal, bool positive);

    std::vector<clang::Token> *expandDefine(const clang::MacroDirective &md);

    bool isMacro(const clang::Token &token);

    bool isFunctionLikeMacro(const clang::Token &token);

    /**
     * @return true if number contained in parser fits into int type
     */
    template <typename signedT, typename unsignedT>
    bool integerFitsIntoType(clang::NumericLiteralParser parser, bool positive);

    std::string getDecimalLiteral(clang::NumericLiteralParser parser);

    std::string getDoubleLiteral(clang::NumericLiteralParser parser);

    bool fitsIntoDouble(clang::NumericLiteralParser parser);
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDER_H
