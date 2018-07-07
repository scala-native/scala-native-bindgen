#ifndef SCALA_NATIVE_BINDGEN_SOURCELOCATION_H
#define SCALA_NATIVE_BINDGEN_SOURCELOCATION_H

#include "Location.h"

class SourceLocation : public Location {
  public:
    explicit SourceLocation(bool isMainFile);

    bool isMainFile() const;

  private:
    bool mainFile;
};

#endif // SCALA_NATIVE_BINDGEN_SOURCELOCATION_H
