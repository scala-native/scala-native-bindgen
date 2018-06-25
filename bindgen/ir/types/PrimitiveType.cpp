#include "PrimitiveType.h"

PrimitiveType::PrimitiveType(std::string type) : type(std::move(type)) {}

std::string PrimitiveType::_str() const { return type; }

std::string PrimitiveType::getType() const { return type; }

bool PrimitiveType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    return str() == type->str();
}
