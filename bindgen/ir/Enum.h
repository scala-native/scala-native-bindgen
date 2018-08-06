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

    bool isAnonymous() const;

    std::shared_ptr<TypeDef> generateTypeDef();

    std::string getName() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e);

    std::string getTypeName() const;

  private:
    std::string name; // might be empty
    std::vector<Enumerator> enumerators;
};

#endif // SCALA_NATIVE_BINDGEN_ENUM_H
