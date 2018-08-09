#ifndef SCALA_NATIVE_BINDGEN_ENUM_H
#define SCALA_NATIVE_BINDGEN_ENUM_H

#include "LocatableType.h"
#include "TypeDef.h"
#include "types/PrimitiveType.h"
#include <vector>

class Enumerator {
  public:
    Enumerator(std::string name, int64_t value);

    std::string getName();

    int64_t getValue();

  private:
    std::string name;
    int64_t value;
};

class Enum : public PrimitiveType, public LocatableType {
  public:
    Enum(std::string name, std::string type,
         std::vector<Enumerator> enumerators,
         std::shared_ptr<Location> location);

    /**
     * @return a string that contains all enumerators.
     *         If enum is not anonymous then enumerators are inside an object
     *         with the same name as enum type.
     */
    std::string getEnumerators() const;

    std::string str(const LocationManager &locationManager) const override;

    std::string getName() const;

    std::string getDefinition() const;

  private:
    std::string name; // non-empty
    std::vector<Enumerator> enumerators;

    std::string getTypeCastSuffix() const;

    std::string getTypeAlias() const;
};

#endif // SCALA_NATIVE_BINDGEN_ENUM_H
