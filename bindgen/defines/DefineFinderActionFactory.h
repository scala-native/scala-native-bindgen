#ifndef SCALA_NATIVE_BINDGEN_DEFINEFINDERACTIONFACTORY_H
#define SCALA_NATIVE_BINDGEN_DEFINEFINDERACTIONFACTORY_H

#include "../ir/IR.h"
#include <clang/Tooling/Tooling.h>

class DefineFinderActionFactory : public clang::tooling::FrontendActionFactory {
  public:
    explicit DefineFinderActionFactory(IR &ir);

    clang::FrontendAction *create() override;

  private:
    IR &ir;
};

#endif // SCALA_NATIVE_BINDGEN_DEFINEFINDERACTIONFACTORY_H
