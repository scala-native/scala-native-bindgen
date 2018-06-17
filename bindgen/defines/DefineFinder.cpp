#include "DefineFinder.h"

#include <clang/Basic/IdentifierTable.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/MacroInfo.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>

DefineFinder::DefineFinder(IR &ir, const clang::CompilerInstance &compiler,
                           clang::Preprocessor &pp)
    : ir(ir), compiler(compiler), pp(pp) {}

void DefineFinder::MacroDefined(const clang::Token &MacroNameTok,
                                const clang::MacroDirective *MD) {
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(MacroNameTok.getLocation()) && MD->isDefined() &&
        !MD->getMacroInfo()->isFunctionLike()) {
        /* save defines only from the given header.
         *
         * Ignore function-like definitions because they usually are meaningful
         * only inside C functions. */

        clang::ArrayRef<clang::Token> tokens = MD->getMacroInfo()->tokens();
        if (tokens.size() != 1) {
            /* process only simple definitions that contain only 1 token */
            return;
        }
        std::string macroName = MacroNameTok.getIdentifierInfo()->getName();

        clang::Token token = tokens[0];
        const clang::Token *finalToken;
        if (token.isAnyIdentifier()) {
            /* current token might be another definition */
            finalToken = getFinalIdentifier(token);
        } else {
            finalToken = &token;
        }
        if (!finalToken) {
            return;
        }
        if (finalToken->isLiteral()) {
            /* might be converted directly to Scala code */
            std::string literal(finalToken->getLiteralData(),
                                finalToken->getLength());
            if (finalToken->getKind() == clang::tok::numeric_constant) {
                addNumericConstantDefine(macroName, literal, finalToken);
            } else if (finalToken->getKind() == clang::tok::string_literal) {
                ir.addLiteralDefine(macroName, "c" + literal, "native.CString");
            } else {
                llvm::errs() << "Warning: type of literal "
                             << finalToken->getName() << " is unsupported\n";
                llvm::errs().flush();
            }
        } else if (finalToken->isAnyIdentifier()) {
            // TODO: save identifier and get its type in ScalaFrontendAction
        }
    }
}

const clang::Token *
DefineFinder::getFinalIdentifier(const clang::Token &token) const {
    assert(token.isAnyIdentifier());
    if (!token.getIdentifierInfo()->hasMacroDefinition()) {
        return &token;
    }
    // TODO: token is another definition. Find the original value
    return nullptr;
}

void DefineFinder::MacroUndefined(const clang::Token &MacroNameTok,
                                  const clang::MacroDefinition &MD) {
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(MacroNameTok.getLocation()) &&
        !MD.getMacroInfo()->isFunctionLike()) {
        std::string macroName = MacroNameTok.getIdentifierInfo()->getName();
        ir.removeDefine(macroName);
    }
}

void DefineFinder::addNumericConstantDefine(const std::string &macroName,
                                            std::string literal,
                                            const clang::Token *finalToken) {
    clang::NumericLiteralParser parser(literal, finalToken->getLocation(), pp);
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
 *
 * Set `literal` and `type` parameters.
 */
void DefineFinder::getTypeOfIntLiteralWithoutEnding(
    clang::NumericLiteralParser parser, std::string &literal,
    std::string &type) {

    bool isSigned = llvm::APSInt(literal).isSigned();

    llvm::APInt intSignedVal(4 * 8, 0, false);
    llvm::APInt intUnsignedVal(4 * 8 - 1, 0, false);
    /* it does not matter if APInt instance is signed or not
     * because when calling GetIntegerValue it will check if unsigned value may
     * fit into the full width defined in APInt */
    if ((!isSigned && !parser.GetIntegerValue(intUnsignedVal)) ||
        (isSigned && !parser.GetIntegerValue(intSignedVal))) {
        type = "native.CInt";
    } else {
        llvm::APInt longSignedVal(8 * 8, 0, false);
        llvm::APInt longUnsignedVal(8 * 8 - 1, 0, false);
        if ((!isSigned && !parser.GetIntegerValue(longUnsignedVal)) ||
            (isSigned && !parser.GetIntegerValue(longSignedVal))) {
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
