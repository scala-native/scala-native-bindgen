#ifndef SCALA_NATIVE_BINDGEN_POINTERTYPE_H
#define SCALA_NATIVE_BINDGEN_POINTERTYPE_H

#include "Type.h"

class PointerType : public Type {
  public:
    explicit PointerType(std::shared_ptr<const Type> type);

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    bool findAllCycles(
        const std::shared_ptr<const Struct> &startStruct, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str(const LocationManager &locationManager) const override;

    bool operator==(const Type &other) const override;

    std::shared_ptr<const Type> unrollTypedefs() const override;

    std::shared_ptr<const Type>
    replaceType(const std::shared_ptr<const Type> &type,
                const std::shared_ptr<const Type> &replacement) const override;

  private:
    std::shared_ptr<const Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_POINTERTYPE_H
