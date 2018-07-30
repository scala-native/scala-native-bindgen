#include "TypeAndName.h"
#include <clang/Tooling/Tooling.h>

TypeAndName::TypeAndName(std::string name, std::shared_ptr<const Type> type)
    : name(std::move(name)), type(std::move(type)) {}

std::shared_ptr<const Type> TypeAndName::getType() const { return type; }

std::string TypeAndName::getName() const { return name; }

void TypeAndName::setType(std::shared_ptr<const Type> type) {
    this->type = type;
}

bool TypeAndName::operator==(const TypeAndName &other) const {
    if (this == &other) {
        return true;
    }
    return name == other.name && *type == *other.type;
}

bool TypeAndName::operator!=(const TypeAndName &other) const {
    return !(*this == other);
}

bool TypeAndName::usesType(const std::shared_ptr<const Type> &type,
                           bool stopOnTypeDefs) const {
    return *this->type == *type ||
           this->type.get()->usesType(type, stopOnTypeDefs);
}
