#ifndef SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
#define SCALA_NATIVE_BINDGEN_TYPEANDNAME_H

#include "types/Type.h"
#include <string>

/**
 * Base class for function parameters, struct fields
 * and type declarations
 */
class TypeAndName {
  public:
    TypeAndName(std::string name, Type *type);

    Type *getType() const;

    void setType(Type *name);

    std::string getName() const;

  protected:
    std::string name;
    Type *type;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
