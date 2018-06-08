#include "TypeAndName.h"

TypeAndName::TypeAndName(std::string name, std::string type)
    : name(std::move(name)), type(std::move(type)) {}

std::string TypeAndName::getType() const { return type; }

std::string TypeAndName::getName() const { return name; }

void TypeAndName::setType(std::string type) { this->type = std::move(type); }
