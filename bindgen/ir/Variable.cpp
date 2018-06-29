#include "Variable.h"

Variable::Variable(const std::string &name, std::shared_ptr<Type> type)
    : TypeAndName(name, type) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Variable &variable) {
    s << "  val " << variable.getName() << ": " << variable.getType()->str()
      << " = native.extern\n";
    return s;
}