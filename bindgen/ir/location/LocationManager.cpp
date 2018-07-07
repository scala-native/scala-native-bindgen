#include "LocationManager.h"
#include "SourceLocation.h"

LocationManager::LocationManager(std::string mainHeaderPath)
    : mainHeaderPath(std::move(mainHeaderPath)) {}

void LocationManager::loadConfig(const std::string &path) {
    // TODO: implement
}

std::shared_ptr<Location>
LocationManager::getLocation(std::string pathToHeader,
                             std::string includeLocation) {
    // TODO: check config
    pathToHeader = realpath(pathToHeader.c_str(), nullptr);
    return std::make_shared<SourceLocation>(pathToHeader == mainHeaderPath);
}
