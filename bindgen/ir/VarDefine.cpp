#include "VarDefine.h"

VarDefine::VarDefine(std::string name, Variable *variable)
    : Define(std::move(name)), variable(variable) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                              const VarDefine &varDefine) {
    s << "  @name(\"" << varDefine.variable->getName() << "\")\n";
    if (varDefine.variable->getIsConst()) {
        s << "  val ";
    } else {
        s << "  def ";
    }
    s << varDefine.getName() << ": " << varDefine.variable->getType()
      << " = native.extern\n";
    return s;
}
