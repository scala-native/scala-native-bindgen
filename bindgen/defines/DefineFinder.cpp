#include "DefineFinder.h"

#include <clang/Basic/IdentifierTable.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/MacroInfo.h>

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
                                            const std::string &literal,
                                            const clang::Token *finalToken) {
    clang::NumericLiteralParser parser(literal, finalToken->getLocation(), pp);
    std::string type;
    if (parser.isIntegerLiteral()) {
        if (parser.isLongLong) {
            return;
        }
        if (parser.isLong) {
            type = "native.CLong";
        } else {
            type = "native.CInt";
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
