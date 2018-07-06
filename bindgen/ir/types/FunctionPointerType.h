#ifndef SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H

#include "Type.h"
#include <vector>

class FunctionPointerType : public Type {
  public:
    FunctionPointerType(
        std::shared_ptr<Type> returnType,
        const std::vector<std::shared_ptr<Type>> &parametersTypes,
        bool isVariadic);

    ~FunctionPointerType() override = default;

    bool usesType(const std::shared_ptr<Type> &type) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

  private:
    std::shared_ptr<Type> returnType;
    std::vector<std::shared_ptr<Type>> parametersTypes;
    bool isVariadic;
};

#endif // SCALA_NATIVE_BINDGEN_FUNCTIONPOINTERTYPE_H
