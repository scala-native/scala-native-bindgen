#ifndef SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
#define SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H

#include "Location.h"
#include <string>
#include <unordered_map>

class LocationManager {
  public:
    explicit LocationManager(std::string mainHeaderPath);

    void loadConfig(const std::string &path);

    bool inMainFile(const Location &location) const;

  private:
    std::string mainHeaderPath;
    std::unordered_map<std::string, std::string> existingBindings;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
