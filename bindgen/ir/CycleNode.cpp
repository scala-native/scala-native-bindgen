#include "CycleNode.h"

CycleNode::CycleNode(const StructOrUnion *structOrUnion, const Field *field)
    : structOrUnion(structOrUnion), field(field) {}
