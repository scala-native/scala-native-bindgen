#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(IR &ir) : ir(ir) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    if (!ir.libObjEmpty() || ir.hasEnums()) {
        llvm::errs() << "IR is not empty. Please use new instance of "
                        "ScalaFrontendActionFactory.\n";
        llvm::errs().flush();
    }
    return new ScalaFrontendAction(ir);
}
