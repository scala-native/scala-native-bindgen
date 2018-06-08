#include "Function.h"
#include "../Utils.h"


Parameter::Parameter(std::string name, std::string type)
        : TypeAndName(std::move(name), std::move(type)) {}

Function::Function(std::string name, std::vector<Parameter> parameters,
                   std::string retType, bool isVariadic)
        : name(std::move(name)), parameters(std::move(parameters)),
          retType(std::move(retType)), isVariadic(isVariadic) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Function &func) {
    s << "  def " << handleReservedWords(func.name)
      << "(";
    std::string sep = "";
    for (const auto &param : func.parameters) {
        s << sep
          << handleReservedWords(param.getName())
          << ": "
          << param.getType();
        sep = ", ";
    }
    if (func.isVariadic) {
        /* the C Iso require at least one argument in a variadic function, so the comma is fine */
        s << ", " << func.getVarargsParameterName() << ": native.CVararg*";
    }
    s << "): "
      << func.retType
      << " = native.extern\n";
    return s;
}

bool Function::usesType(const std::string &type) const {
    if (typeUsesOtherType(retType, type)) {
        return true;
    }
    for (const auto &parameter : parameters) {
        if (typeUsesOtherType(parameter.getType(), type)) {
            return true;
        }
    }
    return false;
}

std::string Function::getName() const {
    return name;
}

std::string Function::getVarargsParameterName() const {
    std::string parameterName = "varArgs";
    int i = 0;
    while (existParameterWithName(parameterName)) {
        parameterName = "varArgs" + std::to_string(i++);
    }
    return parameterName;
}

bool Function::existParameterWithName(const std::string &parameterName) const {
    for (const auto &parameter : parameters) {
        if (parameter.getName() == parameterName) {
            return true;
        }
    }
    return false;
}
