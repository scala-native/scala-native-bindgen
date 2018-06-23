#ifndef SCALA_NATIVE_BINDGEN_POINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_POINTERTYPE_H

#include "Type.h"

class PointerType : public Type {
  public:
    explicit PointerType(Type *type);

    bool usesType(Type *type) const override;

  protected:
    std::string _str() const override;

  private:
    Type *type;
};

#endif // SCALA_NATIVE_BINDGEN_POINTERTYPE_H
