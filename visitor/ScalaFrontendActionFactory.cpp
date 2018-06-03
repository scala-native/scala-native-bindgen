#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(std::string libName)
        : libName(libName),
          ir(libName) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    if (!ir.libObjEmpty() || ir.hasEnums()) {
        std::cerr << "IR is not empty. Please use new instance of ScalaFrontendActionFactory." << std::endl;
    }
    return new ScalaFrontendAction(&ir); // instance will be deleted by LibTooling
}

const IR &ScalaFrontendActionFactory::getIntermediateRepresentation() const {
    return ir;
}
