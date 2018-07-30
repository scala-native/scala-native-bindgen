#include "TypeDef.h"
#include "../Utils.h"
#include "Enum.h"
#include "Struct.h"

TypeDef::TypeDef(std::string name, std::shared_ptr<const Type> type,
                 std::shared_ptr<Location> location)
    : TypeAndName(std::move(name), std::move(type)),
      LocatableType(std::move(location)) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const TypeDef &typeDef) {
    s << "  type " << handleReservedWords(typeDef.name) << " = ";
    if (typeDef.type) {
        s << typeDef.getType()->str();
    } else {
        s << "native.CStruct0 // incomplete type";
    }
    s << "\n";
    return s;
}

bool TypeDef::usesType(const std::shared_ptr<const Type> &type,
                       bool stopOnTypeDefs) const {
    if (stopOnTypeDefs) {
        return false;
    }
    if (!this->type) {
        return false;
    }
    return *this->type == *type || this->type->usesType(type, stopOnTypeDefs);
}

std::string TypeDef::str() const { return handleReservedWords(name); }

bool TypeDef::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<TypeDef>(&other)) {
        auto *typDef = dynamic_cast<const TypeDef *>(&other);
        if (name != typDef->name) {
            return false;
        }
        return *type == *typDef->type;
    }
    return false;
}

std::shared_ptr<Location> TypeDef::getLocation() const {
    if (!type) {
        assert(false && "This method should not be called on typedef that "
                        "references an opaque type because location cannot be "
                        "defined.");
    }
    if (location) {
        /* if typedef is not generated */
        return location;
    }
    auto structOrUnionPointer =
        std::dynamic_pointer_cast<const StructOrUnion>(type);
    if (structOrUnionPointer) {
        return structOrUnionPointer->getLocation();
    }
    auto enumPointer = std::dynamic_pointer_cast<const Enum>(type);
    if (enumPointer) {
        return enumPointer->getLocation();
    }
    assert(false &&
           "Generated typedef may reference only struct, union or enum");
}

bool TypeDef::hasLocation() const { return location || type; }
