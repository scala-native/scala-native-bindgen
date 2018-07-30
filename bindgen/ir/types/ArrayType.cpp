#include "ArrayType.h"
#include "../../Utils.h"
#include "../Struct.h"

ArrayType::ArrayType(std::shared_ptr<const Type> elementsType, uint64_t size)
    : size(size), elementsType(std::move(elementsType)) {}

std::string ArrayType::str() const {
    return "native.CArray[" + elementsType->str() + ", " +
           uint64ToScalaNat(size) + "]";
}

bool ArrayType::usesType(const std::shared_ptr<const Type> &type,
                         bool stopOnTypeDefs) const {
    return *elementsType == *type ||
           elementsType->usesType(type, stopOnTypeDefs);
}

bool ArrayType::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<ArrayType>(&other) && !isInstanceOf<Union>(&other)) {
        auto *arrayType = dynamic_cast<const ArrayType *>(&other);
        if (size != arrayType->size) {
            return false;
        }
        return *elementsType == *arrayType->elementsType;
    }
    return false;
}
