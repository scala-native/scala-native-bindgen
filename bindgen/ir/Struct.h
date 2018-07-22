#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include "CycleNode.h"
#include "TypeAndName.h"
#include "TypeDef.h"
#include "types/ArrayType.h"
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

class StructOrUnion : public virtual Type {
  public:
    StructOrUnion(std::string name, std::vector<std::shared_ptr<Field>> fields,
                  std::shared_ptr<Location> location);

    virtual std::shared_ptr<TypeDef> generateTypeDef() = 0;

    virtual std::string generateHelperClass() const = 0;

    std::string getName() const;

    std::shared_ptr<Location> getLocation() const;

    virtual std::string getTypeAlias() const = 0;

    virtual bool hasHelperMethods() const;

  protected:
    std::string name;
    std::vector<std::shared_ptr<Field>> fields;
    std::shared_ptr<Location> location;

    virtual bool
    usesType(const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
             std::vector<std::shared_ptr<const Type>> &visitedTypes) const;

    /**
     * Scala Native does not support cyclic types.
     * Cycles may contain structs, unions, typedefs, pointer types and function
     * pointer types.
     * Cycle cannot be broken on value type. There exist at least one field in
     * cycle of pointer type / function pointer type (or typedef alias to one of
     * these types) because otherwise cycle produces structs / unions of
     * infinite size.
     *
     * Type of the field may contain a pointer to a struct / union (or typedef
     * alias to this type).
     * The pointer should be replaced with a pointer to Byte if:
     * - field belongs to a cycle
     * - name of the struct/union is the biggest among all structs/unions in the
     *   cycle that also have fields of non-value type.
     *
     * Note: a field may belong to one or more cycles. It is not enough to check
     *       only one of them.
     *
     * @return true if field type should be replaces
     */
    bool shouldFieldBreakCycle(const std::shared_ptr<Field> &field) const;

    /**
     * If the struct/union belongs to one or more cycles that contain
     * startStructOrUnion then field(s) that causes cycle(s) will be added to
     * cycleNode.cycleNodes (as well as all other fields further in this cycle).
     * @param startStructOrUnion struct or union that should belong to found
     *                           cycles.
     * @param visitedTypes is used to avoid endless cycle of function calls in
     *                     the case of cyclic types.
     * @return true if the struct/union belongs to one or more cycles that
     *         contain startStructOrUnion.
     */
    virtual bool
    findAllCycles(const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
                  std::vector<std::shared_ptr<const Type>> &visitedTypes) const;

    /**
     * @return true if current struct/union has the biggest name among all
     *         structs/unions in the cycle that also have fields of non-value
     *         type.
     */
    bool hasBiggestName(const CycleNode &node,
                        std::vector<std::string> vector) const;
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

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

    bool findAllCycles(
        const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
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

    bool findAllCycles(
        const StructOrUnion *startStructOrUnion, CycleNode &cycleNode,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;
};

#endif // SCALA_NATIVE_BINDGEN_STRUCT_H
