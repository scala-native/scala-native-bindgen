#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <string>

/**
 * Base class for types.
 */
class Type {
  public:
    virtual ~Type();

    virtual std::string str() const;

    virtual bool usesType(Type *type) const;

    /**
     * Instances of Enum, Struct, Union and TypeDef are stored in IR so they are
     * deallocated in IR destructor.
     * All other instances of Type are *not* shared between multiple objects and
     * can be deallocated in containing object.
     * @return true if instance can be deallocated.
     */
    virtual bool canBeDeallocated() const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
