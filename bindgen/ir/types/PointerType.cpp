#include "PointerType.h"
#include "../../Utils.h"

PointerType::PointerType(std::shared_ptr<const Type> type)
    : type(std::move(type)) {}

std::string PointerType::str() const {
    return "native.Ptr[" + type->str() + "]";
}

bool PointerType::usesType(const std::shared_ptr<const Type> &type,
                           bool stopOnTypeDefs) const {
    return *this->type == *type ||
           this->type.get()->usesType(type, stopOnTypeDefs);
}

bool PointerType::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<PointerType>(&other)) {
        auto *pointerType = dynamic_cast<const PointerType *>(&other);
        return *type == *pointerType->type;
    }
    return false;
}
