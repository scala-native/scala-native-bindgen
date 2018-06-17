#ifndef SCALA_NATIVE_BINDGEN_DEFINE_H
#define SCALA_NATIVE_BINDGEN_DEFINE_H

#include "TypeAndName.h"

class Define {
  public:
    explicit Define(std::string name);

  protected:
    const std::string name;
};

#endif // SCALA_NATIVE_BINDGEN_DEFINE_H
