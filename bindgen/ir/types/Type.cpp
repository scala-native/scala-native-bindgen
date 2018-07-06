#include "Type.h"

std::string Type::str() const { return ""; }

bool Type::usesType(const std::shared_ptr<Type> &type) const { return false; }

bool Type::operator==(const Type &other) const { return false; }

bool Type::operator!=(const Type &other) const { return !(*this == other); }
