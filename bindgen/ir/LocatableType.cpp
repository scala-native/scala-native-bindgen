#include "LocatableType.h"

LocatableType::LocatableType(std::shared_ptr<Location> location)
    : location(std::move(location)) {}

std::shared_ptr<Location> LocatableType::getLocation() const {
    return location;
}
