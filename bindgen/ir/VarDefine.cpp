#include "VarDefine.h"

VarDefine::VarDefine(std::string name, std::string varName, std::string type,
                     bool isConst)
    : Define(std::move(name)), varName(std::move(varName)),
      type(std::move(type)), isConst(isConst) {}

std::string VarDefine::getVarName() const { return varName; }

std::string VarDefine::getType() const { return type; }

llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                              const VarDefine &varDefine) {
    s << "  @name(\"" << varDefine.getVarName() << "\")\n";
    if (varDefine.isConst) {
        s << "  val ";
    } else {
        s << "  def ";
    }
    s << varDefine.getName() << ": " << varDefine.getType()
      << " = native.extern\n";
    return s;
}
