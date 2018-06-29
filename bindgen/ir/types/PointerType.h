#ifndef SCALA_NATIVE_BINDGEN_POINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_POINTERTYPE_H

#include "Type.h"

class PointerType : public Type {
  public:
    explicit PointerType(std::shared_ptr<Type> type);

    ~PointerType() override;

    bool usesType(std::shared_ptr<Type> type) const override;

    std::string str() const override;

  private:
    std::shared_ptr<Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_POINTERTYPE_H
