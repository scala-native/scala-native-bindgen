#include "PointerType.h"

PointerType::PointerType(std::shared_ptr<Type> type) : type(std::move(type)) {}

std::string PointerType::str() const {
    return "native.Ptr[" + type->str() + "]";
}

bool PointerType::usesType(std::shared_ptr<Type> type) const {
    if (this == type.get()) {
        return true;
    }
    return this->type == type;
}
