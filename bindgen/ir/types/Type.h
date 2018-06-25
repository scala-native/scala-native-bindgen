#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <string>

/**
 * Base class for types.
 */
class Type {
  public:
    std::string str() const;

    virtual bool usesType(Type *type) const;

  protected:
    virtual std::string _str() const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
