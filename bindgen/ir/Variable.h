#ifndef SCALA_NATIVE_BINDGEN_VARIABLE_H
#define SCALA_NATIVE_BINDGEN_VARIABLE_H

#include "TypeAndName.h"

class Variable : public TypeAndName {
  public:
    Variable(const std::string &name, Type *type);
};

#endif // SCALA_NATIVE_BINDGEN_VARIABLE_H
