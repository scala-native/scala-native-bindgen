#ifndef SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
#define SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H

#include "Location.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>

using json = nlohmann::json;

class LocationManager {
  public:
    explicit LocationManager(std::string mainHeaderPath);

    void loadConfig(const std::string &path);

    bool inMainFile(const Location &location) const;

    /**
     * @return true if given type is imported from another Scala object
     */
    bool isImported(const Location &location) const;

    std::string getImportedType(const Location &location,
                                const std::string &name) const;

  private:
    std::string mainHeaderPath;
    json config;

    json getHeaderEntry(const Location &location) const;

    void validateConfig(const json &config) const;

    void validateHeaderEntry(const json &headerEntry) const;

    void validateNames(const json &names) const;

    void validateKeys(const json &object,
                      const std::unordered_set<std::string> &keys) const;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
