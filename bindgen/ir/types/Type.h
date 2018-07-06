#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <memory>
#include <string>

/**
 * Base class for types.
 */
class Type {
  public:
    virtual ~Type() = default;

    virtual std::string str() const;

    virtual bool usesType(const std::shared_ptr<Type> &type) const;

    virtual bool operator==(const Type &other) const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
