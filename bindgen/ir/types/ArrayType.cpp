#include "ArrayType.h"
#include "../../Utils.h"

ArrayType::ArrayType(Type *elementsType, uint64_t size)
    : size(size), elementsType(elementsType) {}

std::string ArrayType::str() const {
    return "native.CArray[" + elementsType->str() + ", " +
           uint64ToScalaNat(size) + "]";
}

bool ArrayType::usesType(Type *type) const {
    return this == type || elementsType == type;
}

ArrayType::~ArrayType() {
    if (elementsType->canBeDeallocated()) {
        delete elementsType;
    }
}
