#include "Variable.h"

Variable::Variable(const std::string &name, std::shared_ptr<Type> type)
    : TypeAndName(name, type) {}
