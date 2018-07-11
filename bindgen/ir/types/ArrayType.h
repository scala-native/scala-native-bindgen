#ifndef SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
#define SCALA_NATIVE_BINDGEN_ARRAYTYPE_H

#include "Type.h"

class ArrayType : public virtual Type {
  public:
    ArrayType(std::shared_ptr<const Type> elementsType, uint64_t size);

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

  private:
    const uint64_t size;
    std::shared_ptr<const Type> elementsType;
};

#endif // SCALA_NATIVE_BINDGEN_ARRAYTYPE_H
