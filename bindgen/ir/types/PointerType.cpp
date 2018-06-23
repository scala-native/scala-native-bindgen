#include "PointerType.h"

PointerType::PointerType(Type *type) : type(type) {}

std::string PointerType::_str() const {
    return "native.Ptr[" + type->str() + "]";
}

bool PointerType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    return this->type == type;
}
