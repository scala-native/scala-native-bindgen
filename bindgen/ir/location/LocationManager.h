#ifndef SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
#define SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H

#include "Location.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

class LocationManager {
  public:
    explicit LocationManager(std::string mainHeaderPath);

    void loadConfig(const std::string &path);

    std::shared_ptr<Location> getLocation(std::string pathToHeader,
                                          std::string includeLocation);

  private:
    std::string mainHeaderPath;

    /**
     * Maps header relative path to Location instance
     */
    std::unordered_map<std::string, Location> config;

    /**
     * If absolute path of a header is a concatenation of an include path and
     * relative path from a config then the header bindings are in
     * corresponding ScalaLocation.
     */
    std::unordered_set<std::string> includePaths;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
