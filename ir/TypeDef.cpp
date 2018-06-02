#include "TypeDef.h"
#include "../Utils.h"

TypeDef::TypeDef(std::string name, std::string type)
        : TypeAndName(std::move(name), std::move(type)) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const TypeDef &typeDef) {
    s << "  type " + handleReservedWords(typeDef.name) + " = " + typeDef.getType() + "\n";
    return s;
}

bool TypeDef::usesType(const std::string &type) const {
    return typeUsesOtherType(this->type, type);
}
