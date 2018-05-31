#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H


#include <string>
#include <clang/AST/Type.h>
#include "TypeAndName.h"


class TypeDef : TypeAndName {
public:
    TypeDef(std::string name, std::string type);

    friend std::ostream &operator <<(std::ostream &s, const TypeDef &type);
};


#endif //SCALA_NATIVE_BINDGEN_TYPEDEF_H
