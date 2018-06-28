#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName, public Type {
  public:
    TypeDef(std::string name, Type *type);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const TypeDef &type);

    bool usesType(Type *type) const override;

    std::string str() const override;

    bool canBeDeallocated() const override;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
