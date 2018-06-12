#ifndef SCALA_NATIVE_BINDGEN_DEFINE_H
#define SCALA_NATIVE_BINDGEN_DEFINE_H

#include "TypeAndName.h"

class Define : TypeAndName {
  public:
    Define(const std::string &name, const std::string &type);
};

#endif // SCALA_NATIVE_BINDGEN_DEFINE_H
