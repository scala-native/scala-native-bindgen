#ifndef SCALA_NATIVE_BINDGEN_ENUM_H
#define SCALA_NATIVE_BINDGEN_ENUM_H

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

class Enum : public PrimitiveType, public std::enable_shared_from_this<Enum> {
  public:
    Enum(std::string name, std::string type,
         std::vector<Enumerator> enumerators,
         std::shared_ptr<Location> location);

    bool isAnonymous() const;

    std::shared_ptr<TypeDef> generateTypeDef();

    std::string getName() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e);

    std::string getTypeAlias() const;

    std::shared_ptr<Location> getLocation();

  private:
    std::string name; // might be empty
    std::vector<Enumerator> enumerators;
    /**
     * nullptr if type is generated.
     */
    std::shared_ptr<Location> location;
};

#endif // SCALA_NATIVE_BINDGEN_ENUM_H
