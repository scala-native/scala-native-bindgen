#ifndef SCALA_NATIVE_BINDGEN_CYCLEDETECTION_H
#define SCALA_NATIVE_BINDGEN_CYCLEDETECTION_H

#include <vector>

class StructOrUnion;
class Field;

struct CycleNode {
    CycleNode(const StructOrUnion *structOrUnion, const Field *field);
    const StructOrUnion *structOrUnion;
    const Field *field;
    std::vector<CycleNode> cycleNodes;
    bool isValueType = false;
};

#endif // SCALA_NATIVE_BINDGEN_CYCLEDETECTION_H
