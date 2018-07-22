#ifndef SCALA_NATIVE_BINDGEN_POINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_POINTERTYPE_H

#include "Type.h"

class PointerType : public Type,
                    public std::enable_shared_from_this<PointerType> {
  public:
    explicit PointerType(std::shared_ptr<const Type> type);

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    bool findAllCycles(
        const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

  private:
    std::shared_ptr<const Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_POINTERTYPE_H
