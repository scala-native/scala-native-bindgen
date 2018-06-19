#include "DefineFinder.h"

#include <llvm/ADT/APInt.h>

DefineFinder::DefineFinder(IR &ir, const clang::CompilerInstance &compiler,
                           clang::Preprocessor &pp)
    : ir(ir), compiler(compiler), pp(pp) {}

void DefineFinder::MacroDefined(const clang::Token &macroNameTok,
                                const clang::MacroDirective *md) {
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(macroNameTok.getLocation()) && md->isDefined() &&
        !md->getMacroInfo()->isFunctionLike()) {
        /* save defines only from the given header.
         *
         * Ignore function-like definitions because they usually are meaningful
         * only inside C functions. */

        std::vector<clang::Token> *tokens = expandDefine(*md);
        if (!tokens) { // there was function-like macro
            return;
        }
        std::string macroName = macroNameTok.getIdentifierInfo()->getName();

        if (tokens->size() == 1 &&
            (*tokens)[0].getKind() == clang::tok::numeric_constant) {
            clang::Token numberToken = (*tokens)[0];
            std::string literal(numberToken.getLiteralData(),
                                numberToken.getLength());
            addNumericConstantDefine(macroName, literal, numberToken);
        } else if (tokens->size() == 2 &&
                   (*tokens)[0].getKind() == clang::tok::minus &&
                   (*tokens)[1].getKind() == clang::tok::numeric_constant) {
            clang::Token numberToken = (*tokens)[1];
            std::string literal(numberToken.getLiteralData(),
                                numberToken.getLength());
            addNumericConstantDefine(macroName, literal, numberToken, false);
        } else if (tokens->size() == 1 &&
                   (*tokens)[0].getKind() == clang::tok::string_literal) {
            clang::Token stringToken = (*tokens)[0];
            std::string literal(stringToken.getLiteralData(),
                                stringToken.getLength());
            ir.addLiteralDefine(macroName, "c" + literal, "native.CString");
        }
        std::free(tokens);
    }
}

/**
 * @return array of tokens. Non of the returned tokens is a macro.
 */
std::vector<clang::Token> *
DefineFinder::expandDefine(const clang::MacroDirective &md) {
    auto *expandedTokens = new std::vector<clang::Token>();
    clang::ArrayRef<clang::Token> tokens = md.getMacroInfo()->tokens();
    for (const auto &token : tokens) {
        if (isMacro(token)) {
            if (isFunctionLikeMacro(token)) {
                /* function-like macros are unsupported */
                return nullptr;
            }
            std::vector<clang::Token> *newTokens = expandDefine(
                *pp.getLocalMacroDirective(token.getIdentifierInfo()));
            if (!newTokens) {
                std::free(expandedTokens);
                return nullptr;
            }
            for (const auto &newToken : *newTokens) {
                (*expandedTokens).push_back(newToken);
            }
            std::free(newTokens);
        } else {
            (*expandedTokens).push_back(token);
        }
    }
    return expandedTokens;
}

bool DefineFinder::isMacro(const clang::Token &token) {
    return token.isAnyIdentifier() &&
           token.getIdentifierInfo()->hasMacroDefinition();
}

bool DefineFinder::isFunctionLikeMacro(const clang::Token &token) {
    clang::MacroDirective *md =
        pp.getLocalMacroDirective(token.getIdentifierInfo());
    return md->getMacroInfo()->isFunctionLike();
}

void DefineFinder::MacroUndefined(const clang::Token &macroNameTok,
                                  const clang::MacroDefinition &md,
                                  const clang::MacroDirective *undef) {
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(macroNameTok.getLocation()) &&
        !md.getMacroInfo()->isFunctionLike()) {
        std::string macroName = macroNameTok.getIdentifierInfo()->getName();
        ir.removeDefine(macroName);
    }
}

void DefineFinder::addNumericConstantDefine(const std::string &macroName,
                                            std::string literal,
                                            const clang::Token &token,
                                            bool positive) {
    clang::NumericLiteralParser parser(literal, token.getLocation(), pp);
    std::string type;
    if (parser.isIntegerLiteral()) {
        std::replace(literal.begin(), literal.end(), 'l', 'L');
        if (parser.isLongLong) {
            /* literal has `LL` ending but `long long` is represented as
             * `Long` in Scala Native */
            literal = literal.substr(0, literal.length() - 1); // remove last L
            type = "native.CLongLong";
        } else if (parser.isLong) {
            /* literal already has `L` ending */
            type = "native.CLong";
        } else {
            /* literal may not have `l` or `ll` ending but still be of long type
             * NumericLiteralParser does not recognize that 10000000000
             * is a long value.
             * Therefore we need to check that value fits into certain number
             * of bits. */
            getTypeOfIntLiteralWithoutEnding(parser, literal, type, positive);
        }
    } else {
        if (parser.isFloatingLiteral()) {
            type = "native.CDouble";
            // TODO: distinguish between float and double
        }
    }
    if (!type.empty()) {
        if (!positive) {
            literal = "-" + literal;
        }
        ir.addLiteralDefine(macroName, literal, type);
    }
}

void DefineFinder::getTypeOfIntLiteralWithoutEnding(
    clang::NumericLiteralParser parser, std::string &literal, std::string &type,
    bool positive) {

    if (fitsIntoType<int, uint>(parser, positive)) {
        type = "native.CInt";
    } else if (fitsIntoType<long, ulong>(parser, positive)) {
        type = "native.CLong";
        literal = literal + "L";
    } else {
        llvm::errs() << "Waring: integer value does not fit into 8 bytes: "
                     << literal << "\n";
        llvm::errs().flush();
        /**
         * `long long` value has mostly the same size as `long`.
         * Moreover in Scala Native the type is represented as `Long`:
         * @code
         * type CLongLong = Long
         * @endcode
         * Therefore the case of `long long` is not considered here.
         */
    }
}

template <typename signedT, typename unsignedT>
bool DefineFinder::fitsIntoType(clang::NumericLiteralParser parser,
                                bool positive) {
    /* absolute value of minimum negative number will not fit
     * into (sizeof(signedT) * 8 - 1) bits */
    llvm::APInt uintValue(sizeof(signedT) * 8, 0, false);
    if (parser.GetIntegerValue(uintValue)) {
        /* absolute value of the number does not fit into (sizeof(signedT) * 8)
         * bits */
        return false;
    }
    auto uval = static_cast<unsignedT>(uintValue.getZExtValue());
    if (positive) {
        return uval <=
               static_cast<unsignedT>(std::numeric_limits<signedT>::max());
    } else {
        return uval <=
               static_cast<unsignedT>(-std::numeric_limits<signedT>::min());
    }
}
