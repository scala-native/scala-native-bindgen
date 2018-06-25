#ifndef SCALA_NATIVE_BINDGEN_VARDEFINE_H
#define SCALA_NATIVE_BINDGEN_VARDEFINE_H

#include "Define.h"
#include <llvm/Support/raw_ostream.h>

class VarDefine : public Define {
  public:
    VarDefine(std::string name, std::string varName, std::string type);

    std::string getVarName() const;

    std::string getType() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const VarDefine &varDefine);

  private:
    std::string varName;
    std::string type;
};

#endif // SCALA_NATIVE_BINDGEN_VARDEFINE_H
