#ifndef SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H
#define SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H


#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include "ScalaFrontendAction.h"

/**
 * Stores instance of IntermediateRepresentation and passes it
 * to a new instance of ScalaFrontendAction when create method
 * is executed.
 */
class ScalaFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    explicit ScalaFrontendActionFactory(std::string libName);

    clang::FrontendAction *create() override;

    const IR &getIntermediateRepresentation() const;

private:
    std::string libName;

    IR ir;

    /**
     * Helps to make sure that action is executed only ones
     * (otherwise `ir` will contain undefined results)
     */
    bool actionExecuted = false;
};


#endif //SCALA_NATIVE_BINDGEN_SCALAFRONTENDACTIONFACTORY_H
