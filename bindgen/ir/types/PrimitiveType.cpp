#include "PrimitiveType.h"
#include "../../Utils.h"
#include "../Enum.h"

PrimitiveType::PrimitiveType(std::string type) : type(std::move(type)) {}

std::string PrimitiveType::str() const { return handleReservedWords(type); }

std::string PrimitiveType::str(const LocationManager &locationManager) const {
    return str();
}

std::string PrimitiveType::getType() const { return type; }

bool PrimitiveType::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    return false;
}

bool PrimitiveType::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<PrimitiveType>(&other) && !isInstanceOf<Enum>(&other)) {
        auto *primitiveType = dynamic_cast<const PrimitiveType *>(&other);
        return type == primitiveType->type;
    }
    return false;
}

std::shared_ptr<const Type> PrimitiveType::unrollTypedefs() const {
    return std::make_shared<PrimitiveType>(type);
}

std::shared_ptr<const Type> PrimitiveType::replaceType(
    const std::shared_ptr<const Type> &type,
    const std::shared_ptr<const Type> &replacement) const {
    return shared_from_this();
}
