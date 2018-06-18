#include "DefineFinder.h"

#include <clang/Basic/IdentifierTable.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/MacroInfo.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>

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

        const clang::Token *token = expandDefine(*md);
        if (!token) {
            return;
        }
        std::string macroName = macroNameTok.getIdentifierInfo()->getName();

        if (token->isLiteral()) {
            /* might be converted directly to Scala code */
            std::string literal(token->getLiteralData(), token->getLength());
            if (token->getKind() == clang::tok::numeric_constant) {
                addNumericConstantDefine(macroName, literal, token);
            } else if (token->getKind() == clang::tok::string_literal) {
                ir.addLiteralDefine(macroName, "c" + literal, "native.CString");
            } else {
                llvm::errs() << "Warning: type of literal " << token->getName()
                             << " is unsupported\n";
                llvm::errs().flush();
            }
        } else if (token->isAnyIdentifier()) {
            // TODO: save identifier and get its type in ScalaFrontendAction
        }
    }
}

/**
 * Follows simple chain of defines.
 *
 * @return token that is not a define
 */
const clang::Token *
DefineFinder::expandDefine(const clang::MacroDirective &md) {
    clang::ArrayRef<clang::Token> tokens = md.getMacroInfo()->tokens();
    if (tokens.size() != 1) {
        /* process only simple definitions that contain 1 token */
        return nullptr;
    }
    clang::Token token = tokens[0];
    if (!(token.isAnyIdentifier() || token.isLiteral())) {
        /* unsupported */
        return nullptr;
    }
    if (token.isLiteral() || !token.getIdentifierInfo()->hasMacroDefinition()) {
        return &(tokens[0]);
    }
    return expandDefine(*pp.getLocalMacroDirective(token.getIdentifierInfo()));
}

void DefineFinder::MacroUndefined(const clang::Token &macroNameTok,
                                  const clang::MacroDefinition &md) {
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(macroNameTok.getLocation()) &&
        !md.getMacroInfo()->isFunctionLike()) {
        std::string macroName = macroNameTok.getIdentifierInfo()->getName();
        ir.removeDefine(macroName);
    }
}

void DefineFinder::addNumericConstantDefine(const std::string &macroName,
                                            std::string literal,
                                            const clang::Token *token) {
    clang::NumericLiteralParser parser(literal, token->getLocation(), pp);
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
            getTypeOfIntLiteralWithoutEnding(parser, literal, type);
        }
    } else {
        if (parser.isFloat) {
            type = "native.CFloat";
            // TODO: distinguish between float and double
        }
    }
    if (!type.empty()) {
        ir.addLiteralDefine(macroName, literal, type);
    }
}

/**
 * Check if literal without `l` or `ll` ending fits into int or long variable.
 * Supports only non-negative numbers
 *
 * Set `literal` and `type` parameters.
 */
void DefineFinder::getTypeOfIntLiteralWithoutEnding(
    clang::NumericLiteralParser parser, std::string &literal,
    std::string &type) {

    llvm::APInt intValue(4 * 8 - 1, 0, false);
    /* check if abs value fits into 4 * 8 - 1 bits */
    if (!parser.GetIntegerValue(intValue)) {
        type = "native.CInt";
    } else {
        llvm::APInt longValue(8 * 8 - 1, 0, false);
        if (!parser.GetIntegerValue(longValue)) {
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
}
