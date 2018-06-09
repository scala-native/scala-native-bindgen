#ifndef SCALA_NATIVE_BINDGEN_ENUM_H
#define SCALA_NATIVE_BINDGEN_ENUM_H

#include "TypeDef.h"
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

class Enum {
  public:
    Enum(std::string name, std::string type,
         std::vector<Enumerator> enumerators);

    bool isAnonymous() const;

    TypeDef generateTypeDef() const;

    std::string getTypeName() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e);

  private:
    std::string name; // might be empty
    std::string type;
    std::vector<Enumerator> enumerators;
};

#endif // SCALA_NATIVE_BINDGEN_ENUM_H
