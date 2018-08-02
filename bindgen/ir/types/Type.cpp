#include "Type.h"
#include <cassert>

CycleNode::CycleNode(std::shared_ptr<const Struct> s, const Field *field)
    : s(std::move(s)), field(field) {}

bool Type::operator!=(const Type &other) const { return !(*this == other); }

bool Type::findAllCycles(
    const std::shared_ptr<const Struct> &startStruct, CycleNode &cycleNode,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    return false;
}

std::shared_ptr<const Type> Type::unrollTypedefs() const { return nullptr; }

std::shared_ptr<const Type>
Type::replaceType(const std::shared_ptr<const Type> &type,
                  const std::shared_ptr<const Type> &replacement) const {
    assert(
        false &&
        "Base class implementation of replaceType method must not be executed");
    return nullptr;
}
