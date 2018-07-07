#include "TypeDef.h"
#include "../Utils.h"
#include "Enum.h"
#include "Struct.h"

TypeDef::TypeDef(std::string name, std::shared_ptr<Type> type,
                 std::shared_ptr<Location> location)
    : TypeAndName(std::move(name), std::move(type)),
      location(std::move(location)) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const TypeDef &typeDef) {
    if (!typeDef.getType()) {
        llvm::errs() << "Error: type declaration for " << typeDef.getName()
                     << " was not found.\n";
        llvm::errs().flush();
        return s;
    }
    s << "  type " + handleReservedWords(typeDef.name) + " = " +
             typeDef.getType()->str() + "\n";
    return s;
}

bool TypeDef::usesType(const std::shared_ptr<Type> &type,
                       bool stopOnTypeDefs) const {
    if (stopOnTypeDefs) {
        return false;
    }
    return *this->type == *type ||
           this->type.get()->usesType(type, stopOnTypeDefs);
}

std::string TypeDef::str() const { return handleReservedWords(name); }

bool TypeDef::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<const TypeDef>(&other)) {
        auto *typDef = dynamic_cast<const TypeDef *>(&other);
        if (name != typDef->name) {
            return false;
        }
        return *type == *typDef->type;
    }
    return false;
}

void TypeDef::setLocation(std::shared_ptr<Location> location) {
    this->location = std::move(location);
}

std::shared_ptr<Location> TypeDef::getLocation() { return location; }
