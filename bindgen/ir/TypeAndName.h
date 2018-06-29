#ifndef SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
#define SCALA_NATIVE_BINDGEN_TYPEANDNAME_H

#include "types/Type.h"
#include <memory>
#include <string>

/**
 * Base class for function parameters, struct fields
 * and type declarations
 */
class TypeAndName {
  public:
    TypeAndName(std::string name, std::shared_ptr<Type> type);

    std::shared_ptr<Type> getType() const;

    void setType(std::shared_ptr<Type> name);

    std::string getName() const;

  protected:
    std::string name;
    std::shared_ptr<Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
