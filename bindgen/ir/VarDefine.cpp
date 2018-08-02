#include "VarDefine.h"

VarDefine::VarDefine(std::string name, std::shared_ptr<Variable> variable)
    : Define(std::move(name)), variable(variable) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                              const VarDefine &varDefine) {
    s << "  @name(\"" << varDefine.variable->getName() << "\")\n"
      << "  val " << varDefine.getName() << ": "
      << varDefine.variable->getType()->str() << " = native.extern\n";
    return s;
}

bool VarDefine::hasIllegalUsageOfOpaqueType() const {
    return variable->hasIllegalUsageOfOpaqueType();
}
