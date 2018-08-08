#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include "../LocationManager.h"
#include <memory>
#include <string>
#include <vector>

class Struct;
class Field;

struct CycleNode {
    CycleNode(std::shared_ptr<const Struct> s, const Field *field);
    std::shared_ptr<const Struct> s;
    const Field *field;
    std::vector<CycleNode> cycleNodes;
    bool isValueType = false;
};

/**
 * Base class for types.
 */
class Type : public std::enable_shared_from_this<Type> {
  public:
    virtual std::string str(const LocationManager &locationManager) const = 0;

    /**
     * @param type search type.
     * @param stopOnTypeDefs if this parameter is true then TypeDefs instances
     *                       will not be checked. This parameter is needed when
     *                       usages of TypeDefs are checked, it helps to avoid
     *                       false positives when usages if aliases for the
     *                       typedef are found.
     *
     * @return true if type uses search type.
     */
    virtual bool
    usesType(const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
             std::vector<std::shared_ptr<const Type>> &visitedTypes) const = 0;

    virtual bool operator==(const Type &other) const = 0;

    virtual bool operator!=(const Type &other) const;

    /**
     * @param startStruct struct that should belong to found
     *                    cycles.
     * @param visitedTypes is used to avoid endless cycle of function calls in
     *                     the case of cyclic types.
     * @return true if current type belongs to one or more cycles that contain
     *         startStruct. If current type is struct or union then
     *         cycleNode is updated (see Record::findAllCycles)
     */
    virtual bool
    findAllCycles(const std::shared_ptr<const Struct> &startStruct,
                  CycleNode &cycleNode,
                  std::vector<std::shared_ptr<const Type>> &visitedTypes) const;

    /**
     * Execution stops at typedefs of structs and unions therefore it cannot
     * stuck in infinite recursion.
     * @return copy of current type in which all typedefs are omitted except
     *         typedefs that wrap structs, unions, enums and opaque types.
     */
    virtual std::shared_ptr<const Type> unrollTypedefs() const;

    /**
     * Execution stops at typedefs of structs and unions therefore it cannot
     * stuck in infinite recursion.
     * @return copy of current type in which given type is replaced with
     *         replacement type.
     */
    virtual std::shared_ptr<const Type>
    replaceType(const std::shared_ptr<const Type> &type,
                const std::shared_ptr<const Type> &replacement) const;

  protected:
    template <typename Derived> std::shared_ptr<Derived> shared_from_base() {
        return std::dynamic_pointer_cast<Derived>(shared_from_this());
    }

    template <typename Derived>
    std::shared_ptr<const Derived> shared_from_base() const {
        return std::dynamic_pointer_cast<const Derived>(shared_from_this());
    }
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
