#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName, public Type {
  public:
    TypeDef(std::string name, std::shared_ptr<Type> type);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const TypeDef &type);

    bool usesType(std::shared_ptr<Type> type) const override;

    std::string str() const override;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
