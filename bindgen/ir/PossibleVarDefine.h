#ifndef SCALA_NATIVE_BINDGEN_POSSIBLEVARDEFINE_H
#define SCALA_NATIVE_BINDGEN_POSSIBLEVARDEFINE_H

#include "Define.h"

/**
 * Stores name of possible variable.
 */
class PossibleVarDefine : public Define {
  public:
    PossibleVarDefine(const std::string &name, const std::string &variableName);

    std::string getVariableName() const;

  private:
    std::string variableName;
};

#endif // SCALA_NATIVE_BINDGEN_POSSIBLEVARDEFINE_H
