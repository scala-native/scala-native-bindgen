#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H


#include <string>
#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>


class TypeDef : TypeAndName {
public:
    TypeDef(std::string name, std::string type);

    friend llvm::raw_ostream &operator <<(llvm::raw_ostream &s, const TypeDef &type);
};


#endif //SCALA_NATIVE_BINDGEN_TYPEDEF_H
