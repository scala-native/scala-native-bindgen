#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include "Record.h"

#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22

class Struct : public Record {
  public:
    Struct(std::string name, std::vector<std::shared_ptr<Field>> fields,
           uint64_t typeSize, std::shared_ptr<Location> location, bool isPacked,
           bool isBitField);

    std::shared_ptr<TypeDef> generateTypeDef() override;

    std::string
    generateHelperClass(const LocationManager &locationManager) const override;

    std::string getTypeName() const override;

    /**
     * @return true if helper methods will be generated for this struct
     */
    bool hasHelperMethods() const override;

    std::string str(const LocationManager &locationManager) const override;

    bool operator==(const Type &other) const override;

    /**
     * If the struct belongs to one or more cycles that contain
     * startStruct then field(s) that causes cycle(s) will be added to
     * cycleNode.cycleNodes (as well as all other fields further in this cycle).
     * @param startStruct struct that should belong to found cycles.
     * @param visitedTypes is used to avoid endless cycle of function calls in
     *                     the case of cyclic types.
     * @return true if the struct belongs to one or more cycles that
     *         contain startStruct.
     */
    bool findAllCycles(
        const std::shared_ptr<const Struct> &startStruct, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  private:
    /** type size is needed if number of fields is bigger than 22 */
    uint64_t typeSize;
    bool isPacked;
    /** true if at least one field is bit field */
    bool hasBitField;

    bool isRepresentedAsStruct() const;

    /**
     * @return helper class methods for struct that is represented as CStruct.
     */
    std::string generateHelperClassMethodsForStructRepresentation(
        const LocationManager &locationManager) const;

    /**
     * @return helper class methods for struct that is represented as CArray.
     */
    std::string generateHelperClassMethodsForArrayRepresentation(
        const LocationManager &locationManager) const;

    std::string generateSetterForStructRepresentation(
        unsigned fieldIndex, const LocationManager &locationManager) const;

    std::string generateGetterForStructRepresentation(
        unsigned fieldIndex, const LocationManager &locationManager) const;

    std::string generateSetterForArrayRepresentation(
        unsigned fieldIndex, const LocationManager &locationManager) const;

    std::string generateGetterForArrayRepresentation(
        unsigned fieldIndex, const LocationManager &locationManager) const;

    /**
     * This function is used to get type replacement for a field that should
     * break Scala Native types cycle.
     * @return copy of type where:
     *         - all typedefs are omitted
     *         - pointers to structTypesThatShouldBeReplaced are replaced with
     *           pointers to Byte
     *         - structTypesThatShouldBeReplaced value type is replaced with
     *           CStruct0 (it may happen in function pointer types)
     */
    std::shared_ptr<const Type>
    getTypeReplacement(std::shared_ptr<const Type> type,
                       std::vector<std::shared_ptr<const Struct>>
                           structTypesThatShouldBeReplaced) const;

    /**
     * Scala Native does not support cyclic types.
     * Cycles may contain structs, typedefs, pointer types and function
     * pointer types.
     * Unions are represented as arrays therefore they cannot belong to cycle.
     * Cycle cannot be broken on value type. There exist at least one field in
     * cycle of pointer type / function pointer type (or typedef alias to one of
     * these types) because otherwise cycle produces structs of
     * infinite size.
     *
     * Type of the field may contain a pointer to a struct (or typedef
     * alias to this type).
     * The pointer should be replaced with a pointer to Byte in following case:
     * - field belongs to a cycle
     * - name of the struct is the biggest among all structs in the
     *   cycle that also have fields of non-value type.
     *
     * Note: a field may belong to one or more cycles. It is not enough to check
     *       only one of cycles.
     *
     * @return vector of structs that should be replaced by Byte type in order
     *         to break cycle.
     */
    std::vector<std::shared_ptr<const Struct>>
    shouldFieldBreakCycle(const std::shared_ptr<Field> &field) const;

    /**
     * @return true if current struct has the biggest name among all structs in
     *         the cycle that also have fields of non-value type.
     */
    bool hasBiggestName(const CycleNode &node,
                        std::vector<std::string> namesInCycle) const;
};

#endif // SCALA_NATIVE_BINDGEN_STRUCT_H
