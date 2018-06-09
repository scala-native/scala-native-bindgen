#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(std::string libName)
    : libName(libName), ir(libName) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    if (!ir.libObjEmpty() || ir.hasEnums()) {
        llvm::errs() << "IR is not empty. Please use new instance of "
                        "ScalaFrontendActionFactory.\n";
        llvm::errs().flush();
    }
    return new ScalaFrontendAction(
        &ir); // instance will be deleted by LibTooling
}

const IR &ScalaFrontendActionFactory::getIntermediateRepresentation() const {
    return ir;
}
