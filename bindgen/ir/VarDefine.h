#ifndef SCALA_NATIVE_BINDGEN_VARDEFINE_H
#define SCALA_NATIVE_BINDGEN_VARDEFINE_H

#include "Define.h"

class VarDefine : public Define {
  public:
    VarDefine(std::string name, std::string varName, std::string type);

    std::string getVarName() const;

    std::string getType() const;

  private:
    std::string varName;
    std::string type;
};

#endif // SCALA_NATIVE_BINDGEN_VARDEFINE_H
