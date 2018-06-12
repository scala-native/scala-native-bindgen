#include "DefineFinder.h"

#include <clang/Basic/IdentifierTable.h>
#include <clang/Lex/MacroInfo.h>

DefineFinder::DefineFinder(IR &ir, const clang::CompilerInstance &compiler)
    : ir(ir), compiler(compiler) {}

void DefineFinder::MacroDefined(const clang::Token &MacroNameTok,
                                const clang::MacroDirective *MD) {
    std::string name = MacroNameTok.getIdentifierInfo()->getName();
    clang::SourceManager &sm = compiler.getSourceManager();
    if (sm.isWrittenInMainFile(MacroNameTok.getLocation())) {
        llvm::errs() << name << "\n";
        for (const auto &tok : MD->getMacroInfo()->tokens()) {
            llvm::errs() << tok.getName() << "\n";
        }
    }
}
