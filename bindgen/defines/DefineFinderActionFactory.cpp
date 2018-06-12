#include "DefineFinderActionFactory.h"
#include "DefineFinderAction.h"

DefineFinderActionFactory::DefineFinderActionFactory(IR &ir) : ir(ir) {}

clang::FrontendAction *DefineFinderActionFactory::create() {
    return new DefineFinderAction(ir);
}
