#include "ArrayType.h"
#include "../../Utils.h"
#include "../Struct.h"

ArrayType::ArrayType(std::shared_ptr<const Type> elementsType, uint64_t size)
    : size(size), elementsType(std::move(elementsType)) {}

std::string ArrayType::str() const {
    return "native.CArray[" + elementsType->str() + ", " +
           uint64ToScalaNat(size) + "]";
}

bool ArrayType::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    if (contains(this, visitedTypes)) {
        return false;
    }
    visitedTypes.push_back(shared_from_this());
    bool result = *elementsType == *type ||
                  elementsType->usesType(type, stopOnTypeDefs, visitedTypes);
    if (!result) {
        /* current ArrayType instance should not be in the path to search
         * type */
        visitedTypes.pop_back();
    }
    return result;
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
