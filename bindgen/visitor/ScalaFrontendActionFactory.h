#ifndef SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H
#define SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H

#include "ScalaFrontendAction.h"
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

/**
 * Stores instance of IR and passes it
 * to a new instance of ScalaFrontendAction when create method
 * is executed.
 */
class ScalaFrontendActionFactory
    : public clang::tooling::FrontendActionFactory {
  public:
    explicit ScalaFrontendActionFactory(IR &ir);

    clang::FrontendAction *create() override;

  private:
    IR &ir;
};

#endif // SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H
