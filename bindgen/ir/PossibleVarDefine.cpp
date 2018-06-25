#include "PossibleVarDefine.h"

PossibleVarDefine::PossibleVarDefine(const std::string &name,
                                     const std::string &variableName)
    : Define(name), variableName(variableName) {}

std::string PossibleVarDefine::getVariableName() const { return variableName; }
