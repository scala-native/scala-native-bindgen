#ifndef SCALA_NATIVE_BINDGEN_SCALALOCATION_H
#define SCALA_NATIVE_BINDGEN_SCALALOCATION_H

#include "Location.h"
#include <string>

class ScalaLocation : public Location {
  public:
    ScalaLocation(std::string packageName, std::string objectName);

  private:
    std::string packageName;
    std::string objectName;
};

#endif // SCALA_NATIVE_BINDGEN_SCALALOCATION_H
