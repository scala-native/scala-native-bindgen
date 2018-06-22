#include "VarDefine.h"

VarDefine::VarDefine(std::string name, std::string varName, std::string type)
    : Define(std::move(name)), varName(std::move(varName)),
      type(std::move(type)) {}

std::string VarDefine::getVarName() const { return varName; }

std::string VarDefine::getType() const { return type; }
