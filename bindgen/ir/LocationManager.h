#ifndef SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
#define SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H

#include "Location.h"
#include <map>
#include <string>
#include <vector>

struct HeaderEntryName {
    std::string original;
    std::string target;
};

struct HeaderEntry {
    std::string path;
    std::string object;
    std::vector<HeaderEntryName> names;
};

class LocationManager {
  public:
    explicit LocationManager(std::string mainHeaderPath);

    bool loadConfig(const std::string &path);

    bool inMainFile(const Location &location) const;

    /**
     * @return true if given type is imported from another Scala object
     */
    bool isImported(const Location &location) const;

    std::string getImportedType(const Location &location,
                                const std::string &name) const;

  private:
    std::vector<HeaderEntry>::size_type
    getHeaderEntryIndex(const Location &location) const;
    std::vector<HeaderEntry> headerEntries;
    std::string mainHeaderPath;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATIONMANAGER_H
