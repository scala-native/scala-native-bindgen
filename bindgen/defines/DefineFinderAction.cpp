#include "DefineFinderAction.h"

DefineFinderAction::DefineFinderAction(IR &ir) : ir(ir) {}

void DefineFinderAction::ExecuteAction() {
    getCompilerInstance().getPreprocessor().addPPCallbacks(
        std::unique_ptr<clang::PPCallbacks>(
            new DefineFinder(ir, getCompilerInstance())));

    clang::PreprocessOnlyAction::ExecuteAction();
}
