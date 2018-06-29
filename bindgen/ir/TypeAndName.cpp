#include "TypeAndName.h"
#include <clang/Tooling/Tooling.h>

TypeAndName::TypeAndName(std::string name, std::shared_ptr<Type> type)
    : name(std::move(name)), type(type) {}

std::shared_ptr<Type> TypeAndName::getType() const { return type; }

std::string TypeAndName::getName() const { return name; }

void TypeAndName::setType(std::shared_ptr<Type> type) { this->type = type; }
