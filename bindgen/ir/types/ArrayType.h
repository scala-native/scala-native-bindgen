#ifndef SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
#define SCALA_NATIVE_BINDGEN_ARRAYTYPE_H

#include "Type.h"
#include <cstdint>
#include <string>

class ArrayType : public Type {
  public:
    ArrayType(Type *elementsType, uint64_t size);

    bool usesType(Type *type) const override;

  protected:
    std::string _str() const override;

  private:
    const uint64_t size;
    Type *elementsType;
};

#endif // SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
