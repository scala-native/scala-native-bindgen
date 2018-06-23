#ifndef SCALA_NATIVE_BINDGEN_DEFINE_H
#define SCALA_NATIVE_BINDGEN_DEFINE_H

#include <string>

class Define {
  public:
    explicit Define(std::string name);

    std::string getName();

  protected:
    std::string name;
};

#endif // SCALA_NATIVE_BINDGEN_DEFINE_H
