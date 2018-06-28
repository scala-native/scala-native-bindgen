#include "PrimitiveType.h"
#include "../../Utils.h"

PrimitiveType::PrimitiveType(std::string type) : type(std::move(type)) {}

std::string PrimitiveType::str() const { return handleReservedWords(type); }

std::string PrimitiveType::getType() const { return type; }

bool PrimitiveType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    return str() == type->str();
}
