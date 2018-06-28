#include "Type.h"

std::string Type::str() const { return ""; }

bool Type::usesType(Type *type) const { return false; }

bool Type::canBeDeallocated() const { return true; }

Type::~Type() = default;
