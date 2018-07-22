#include "Type.h"

bool Type::operator!=(const Type &other) const { return !(*this == other); }

bool Type::findAllCycles(
    const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    return false;
}
