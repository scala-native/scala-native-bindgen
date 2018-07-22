#include "PointerType.h"
#include "../../Utils.h"

PointerType::PointerType(std::shared_ptr<const Type> type)
    : type(std::move(type)) {}

std::string PointerType::str() const {
    return "native.Ptr[" + type->str() + "]";
}

bool PointerType::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    if (contains(this, visitedTypes)) {
        return false;
    }
    visitedTypes.push_back(shared_from_this());
    bool result = *this->type == *type ||
                  this->type->usesType(type, stopOnTypeDefs, visitedTypes);
    if (!result) {
        /* current PointerType instance should not be in the path to search
         * type */
        visitedTypes.pop_back();
    }
    return result;
}

bool PointerType::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<PointerType>(&other)) {
        auto *pointerType = dynamic_cast<const PointerType *>(&other);
        return *type == *pointerType->type;
    }
    return false;
}

bool PointerType::findAllCycles(
    const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    if (contains(this, visitedTypes)) {
        return false;
    }
    visitedTypes.push_back(shared_from_this());
    bool result =
        this->type->findAllCycles(startStructOrUnion, cycleNode, visitedTypes);
    visitedTypes.pop_back();
    return result;
}
