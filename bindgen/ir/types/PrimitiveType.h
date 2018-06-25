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

    bool usesType(Type *type) const override;

  private:
    std::string type;

    std::string _str() const override;
};

#endif // SCALA_NATIVE_BINDGEN_SIMPLETYPE_H
