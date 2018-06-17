#include "DefineFinder.h"

#include <clang/Basic/IdentifierTable.h>
#include <clang/Lex/MacroInfo.h>

DefineFinder::DefineFinder(IR &ir, const clang::CompilerInstance &compiler)
    : ir(ir), compiler(compiler) {}

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
            switch (finalToken->getKind()) {
            case clang::tok::numeric_constant:
                ir.addLiteralDefine(macroName, literal);
                break;
            case clang::tok::string_literal:
                ir.addLiteralDefine(macroName, "c" + literal, "native.CString");
                break;
            default:
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
