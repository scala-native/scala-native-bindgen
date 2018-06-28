#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <string>

/**
 * Base class for types.
 */
class Type {
  public:
    virtual std::string str() const;

    virtual bool usesType(Type *type) const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
