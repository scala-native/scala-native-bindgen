#ifndef SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
#define SCALA_NATIVE_BINDGEN_ARRAYTYPE_H

#include "Type.h"

class ArrayType : public Type {
  public:
    ArrayType(Type *elementsType, uint64_t size);

    bool usesType(Type *type) const override;

    std::string str() const override;

  private:
    const uint64_t size;
    Type *elementsType;
};

#endif // SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
