#include "ScalaFrontendActionFactory.h"
#include "ScalaFrontendAction.h"

ScalaFrontendActionFactory::ScalaFrontendActionFactory(
    IR &ir, LocationManager &locationManager)
    : ir(ir), locationManager(locationManager) {}

clang::FrontendAction *ScalaFrontendActionFactory::create() {
    return new ScalaFrontendAction(ir, locationManager);
}
