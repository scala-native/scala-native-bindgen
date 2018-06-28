#include "PointerType.h"

PointerType::PointerType(Type *type) : type(type) {}

std::string PointerType::str() const {
    return "native.Ptr[" + type->str() + "]";
}

bool PointerType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    return this->type == type;
}

PointerType::~PointerType() {
    if (type->canBeDeallocated()) {
        delete type;
    }
}
