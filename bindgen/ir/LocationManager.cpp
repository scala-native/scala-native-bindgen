#include "LocationManager.h"

LocationManager::LocationManager(std::string mainHeaderPath)
    : mainHeaderPath(std::move(mainHeaderPath)) {}

void LocationManager::loadConfig(const std::string &path) {
    // TODO: implement
}

bool LocationManager::inMainFile(const Location &location) const {
    return location.getPath() == mainHeaderPath;
}
