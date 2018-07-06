#include "TypeAndName.h"
#include <clang/Tooling/Tooling.h>

TypeAndName::TypeAndName(std::string name, std::shared_ptr<Type> type)
    : name(std::move(name)), type(std::move(type)) {}

std::shared_ptr<Type> TypeAndName::getType() const { return type; }

std::string TypeAndName::getName() const { return name; }

void TypeAndName::setType(std::shared_ptr<Type> type) { this->type = type; }

bool TypeAndName::operator==(const TypeAndName &other) const {
    return name == other.name && *type == *other.type;
}
