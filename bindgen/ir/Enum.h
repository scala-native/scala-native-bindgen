#ifndef SCALA_NATIVE_BINDGEN_ENUM_H
#define SCALA_NATIVE_BINDGEN_ENUM_H

#include "TypeDef.h"
#include "types/SimpleType.h"
#include <llvm/Support/raw_ostream.h>
#include <string>
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

class Enum : public SimpleType {
  public:
    Enum(std::string name, std::string type,
         std::vector<Enumerator> enumerators);

    bool isAnonymous() const;

    TypeDef *generateTypeDef();

    std::string getName() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e);

  private:
    std::string name; // might be empty
    std::vector<Enumerator> enumerators;
};

#endif // SCALA_NATIVE_BINDGEN_ENUM_H
