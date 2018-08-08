#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "LocatableType.h"
#include "Location.h"
#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName, public LocatableType {
  public:
    /**
     * @param location nullptr if typedef is generated
     */
    TypeDef(std::string name, std::shared_ptr<const Type> type,
            std::shared_ptr<Location> location);

    std::string getDefinition(const LocationManager &locationManager) const;

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str(const LocationManager &locationManager) const override;

    bool operator==(const Type &other) const override;

    /**
     * @return true if the typedef is not generated alias for opaque type.
     */
    bool hasLocation() const;

    /**
     * @return true if typedef directly references an opaque type
     */
    bool wrapperForOpaqueType() const;

    bool findAllCycles(
        const std::shared_ptr<const Struct> &startStruct, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::shared_ptr<const Type> unrollTypedefs() const override;

    std::shared_ptr<const Type>
    replaceType(const std::shared_ptr<const Type> &type,
                const std::shared_ptr<const Type> &replacement) const override;

    /**
     * @return location of the typedef is it is not generated otherwise
     *         location of referenced record.
     */
    std::shared_ptr<Location> getLocation() const override;

  private:
    bool isGenerated() const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
