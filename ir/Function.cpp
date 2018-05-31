#include "Function.h"
#include "../Utils.h"
#include <utility>


Parameter::Parameter(std::string name, std::string type)
        : TypeAndName(std::move(name), std::move(type)) {}

Function::Function(std::string name, std::vector<Parameter> parameters,
                   std::string retType, bool isVariadic)
        : name(std::move(name)), parameters(std::move(parameters)),
          retType(std::move(retType)), isVariadic(isVariadic) {}

std::ostream &operator<<(std::ostream &s, const Function &func) {
    s << "  def " << handleReservedWords(func.name)
      << "(";
    auto paramsCount = static_cast<int>(func.parameters.size());
    for (int i = 0; i < paramsCount; ++i) {
        const Parameter &param = func.parameters[i];
        s << handleReservedWords(param.getName())
          << ": "
          << param.getType();
        if (i < paramsCount - 1) {
            s << ", ";
        }
    }
    if (func.isVariadic) {
        /* the C Iso require at least one argument in a variadic function, so the comma is fine */
        s << ", varArgs: native.CVararg*";
    }
    s << "): "
      << func.retType
      << " = native.extern\n";
    return s;
}
