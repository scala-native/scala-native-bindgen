#ifndef SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H

#include "Type.h"
#include <vector>

class FunctionPointerType : public Type {
  public:
    FunctionPointerType(Type *returnType,
                        const std::vector<Type *> &parametersTypes,
                        bool isVariadic);

    ~FunctionPointerType() override;

    bool usesType(Type *type) const override;

    std::string str() const override;

  private:
    Type *returnType;
    std::vector<Type *> parametersTypes;
    bool isVariadic;
};

#endif // SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H
