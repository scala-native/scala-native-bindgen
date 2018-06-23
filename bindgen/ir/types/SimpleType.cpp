#include "SimpleType.h"

SimpleType::SimpleType(std::string type) : type(std::move(type)) {}

std::string SimpleType::_str() const { return type; }

std::string SimpleType::getType() const { return type; }

bool SimpleType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    return str() == type->str();
}
