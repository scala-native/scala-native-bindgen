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
    TypeAndName(std::string name, std::shared_ptr<const Type> type);

    std::shared_ptr<const Type> getType() const;

    void setType(std::shared_ptr<const Type> name);

    std::string getName() const;

    bool operator==(const TypeAndName &other) const;

    bool operator!=(const TypeAndName &other) const;

    bool usesType(const std::shared_ptr<const Type> &type,
                  bool stopOnTypeDefs) const;

  protected:
    std::string name;
    std::shared_ptr<const Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
