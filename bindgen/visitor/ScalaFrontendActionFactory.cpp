#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(IR &ir) : ir(ir) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    return new ScalaFrontendAction(ir);
}
