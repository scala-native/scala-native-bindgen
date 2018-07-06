#ifndef SCALA_NATIVE_BINDGEN_SIMPLETYPE_H
#define SCALA_NATIVE_BINDGEN_SIMPLETYPE_H

#include "Type.h"
#include <string>

/**
 * For example native.CInt
 */
class PrimitiveType : public Type {
  public:
    explicit PrimitiveType(std::string type);

    std::string getType() const;

    bool usesType(const std::shared_ptr<Type> &type) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

  private:
    std::string type;
};

#endif // SCALA_NATIVE_BINDGEN_SIMPLETYPE_H
