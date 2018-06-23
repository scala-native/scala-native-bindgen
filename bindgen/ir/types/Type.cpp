#include "Type.h"
#include "../../Utils.h"

std::string Type::str() const { return handleReservedWords(_str()); }

std::string Type::_str() const { return ""; }

bool Type::usesType(Type *type) const { return false; }
