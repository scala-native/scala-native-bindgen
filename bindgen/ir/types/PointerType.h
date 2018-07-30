#ifndef SCALA_NATIVE_BINDGEN_POINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_POINTERTYPE_H

#include "Type.h"

class PointerType : public Type {
  public:
    explicit PointerType(std::shared_ptr<const Type> type);

    bool usesType(const std::shared_ptr<const Type> &type,
                  bool stopOnTypeDefs) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

  private:
    std::shared_ptr<const Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_POINTERTYPE_H
