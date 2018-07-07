#include "ScalaLocation.h"

ScalaLocation::ScalaLocation(std::string packageName, std::string objectName)
    : packageName(std::move(packageName)), objectName(std::move(objectName)) {}
