#include "Variable.h"

Variable::Variable(const std::string &name, const std::string &type,
                   bool isConst)
    : TypeAndName(name, type), isConst(isConst) {}

bool Variable::getIsConst() { return isConst; }
