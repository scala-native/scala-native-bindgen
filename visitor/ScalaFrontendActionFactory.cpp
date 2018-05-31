#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(std::string libName)
        : libName(libName),
          ir(libName) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    if (actionExecuted) {
        std::cerr << "Action was already executed";
    }
    actionExecuted = true;
    return new ScalaFrontendAction(&ir); // instance will be deleted by LibTooling
}

const IR &ScalaFrontendActionFactory::getIntermediateRepresentation() const {
    return ir;
}
