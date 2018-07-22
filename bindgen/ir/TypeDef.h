#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "Location.h"
#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName,
                public Type,
                public std::enable_shared_from_this<TypeDef> {
  public:
    TypeDef(std::string name, std::shared_ptr<const Type> type,
            std::shared_ptr<Location> location);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const TypeDef &type);

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

    std::shared_ptr<Location> getLocation() const;

    bool findAllCycles(
        const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  private:
    /**
     * nullptr if type is generated.
     */
    std::shared_ptr<Location> location;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
