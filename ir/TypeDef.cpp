#include "TypeDef.h"
#include "../Utils.h"

TypeDef::TypeDef(std::string name, std::string type)
        : TypeAndName(std::move(name), std::move(type)) {}

std::ostream &operator<<(std::ostream &s, const TypeDef &typeDef) {
    s << "  type " + handleReservedWords(typeDef.name) + " = " + typeDef.getType() + "\n";
    return s;
}
