#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include "LocatableType.h"
#include "TypeAndName.h"
#include "TypeDef.h"
#include "types/ArrayType.h"
#include "types/PointerType.h"
#include <string>
#include <vector>

#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22

class Field : public TypeAndName {
  public:
    Field(std::string name, std::shared_ptr<const Type> type);

    Field(std::string name, std::shared_ptr<const Type> type,
          uint64_t offsetInBits);

    uint64_t getOffsetInBits() const;

  protected:
    /**
     * Offset in bytes from address of struct/union.
     */
    uint64_t offsetInBits = 0;
};

class StructOrUnion : public LocatableType {
  public:
    StructOrUnion(std::string name, std::vector<std::shared_ptr<Field>> fields,
                  std::shared_ptr<Location> location);

    virtual std::shared_ptr<TypeDef> generateTypeDef() = 0;

    virtual std::string generateHelperClass() const = 0;

    std::string getName() const;

    virtual std::string getTypeAlias() const = 0;

    virtual bool hasHelperMethods() const;

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  protected:
    std::string name;
    std::vector<std::shared_ptr<Field>> fields;
};

class Struct : public StructOrUnion {
  public:
    Struct(std::string name, std::vector<std::shared_ptr<Field>> fields,
           uint64_t typeSize, std::shared_ptr<Location> location, bool isPacked,
           bool isBitField);

    std::shared_ptr<TypeDef> generateTypeDef() override;

    std::string generateHelperClass() const override;

    std::string getTypeAlias() const override;

    /**
     * @return true if helper methods will be generated for this struct
     */
    bool hasHelperMethods() const override;

    std::string str() const override;

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
    std::string generateHelperClassMethodsForStructRepresentation() const;

    /**
     * @return helper class methods for struct that is represented as CArray.
     */
    std::string generateHelperClassMethodsForArrayRepresentation() const;

    std::string
    generateSetterForStructRepresentation(unsigned fieldIndex) const;

    std::string
    generateGetterForStructRepresentation(unsigned fieldIndex) const;

    std::string generateSetterForArrayRepresentation(unsigned fieldIndex) const;

    std::string generateGetterForArrayRepresentation(unsigned fieldIndex) const;

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

class Union : public StructOrUnion, public ArrayType {
  public:
    Union(std::string name, std::vector<std::shared_ptr<Field>> fields,
          uint64_t maxSize, std::shared_ptr<Location> location);

    std::shared_ptr<TypeDef> generateTypeDef() override;

    std::string generateHelperClass() const override;

    bool operator==(const Type &other) const override;

    std::string getTypeAlias() const override;

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  private:
    std::string generateGetter(const std::shared_ptr<Field> &field) const;

    std::string generateSetter(const std::shared_ptr<Field> &field) const;
};

#endif // SCALA_NATIVE_BINDGEN_STRUCT_H
