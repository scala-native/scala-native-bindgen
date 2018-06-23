#include "TypeAndName.h"

TypeAndName::TypeAndName(std::string name, Type *type)
    : name(std::move(name)), type(type) {}

Type *TypeAndName::getType() const { return type; }

std::string TypeAndName::getName() const { return name; }

void TypeAndName::setType(Type *type) { this->type = type; }
