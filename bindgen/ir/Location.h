#ifndef SCALA_NATIVE_BINDGEN_LOCATION_H
#define SCALA_NATIVE_BINDGEN_LOCATION_H

#include <string>

class Location {
  public:
    Location(std::string path, int lineNumber);

    std::string getPath() const;

    int getLineNumber() const;

  private:
    std::string path; // may be empty
    int lineNumber;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATION_H
