#include "TypeDef.h"
#include "../Utils.h"

TypeDef::TypeDef(std::string name, Type *type)
    : TypeAndName(std::move(name), type) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const TypeDef &typeDef) {
    s << "  type " + handleReservedWords(typeDef.name) + " = " +
             typeDef.getType()->str() + "\n";
    return s;
}

bool TypeDef::usesType(Type *type) const {
    return this == type || this->type == type;
}

std::string TypeDef::str() const { return handleReservedWords(name); }

bool TypeDef::canBeDeallocated() const { return false; }
