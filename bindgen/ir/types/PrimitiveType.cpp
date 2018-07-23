#include "PrimitiveType.h"
#include "../../Utils.h"
#include "../Enum.h"

PrimitiveType::PrimitiveType(std::string type) : type(std::move(type)) {}

std::string PrimitiveType::str() const { return handleReservedWords(type); }

std::string PrimitiveType::getType() const { return type; }

bool PrimitiveType::usesType(const std::shared_ptr<Type> &type,
                             bool stopOnTypeDefs) const {
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
