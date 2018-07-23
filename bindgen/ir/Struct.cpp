#include "Struct.h"
#include "../Utils.h"
#include "types/ArrayType.h"
#include "types/FunctionPointerType.h"
#include "types/PointerType.h"
#include "types/PrimitiveType.h"
#include <sstream>

Field::Field(std::string name, std::shared_ptr<const Type> type)
    : TypeAndName(std::move(name), std::move(type)) {}

Field::Field(std::string name, std::shared_ptr<const Type> type,
             uint64_t offsetInBits)
    : TypeAndName(std::move(name), std::move(type)),
      offsetInBits(offsetInBits) {}

uint64_t Field::getOffsetInBits() const { return offsetInBits; }

StructOrUnion::StructOrUnion(std::string name,
                             std::vector<std::shared_ptr<Field>> fields,
                             std::shared_ptr<Location> location)
    : name(std::move(name)), fields(std::move(fields)),
      location(std::move(location)) {}

std::string StructOrUnion::getName() const { return name; }

std::shared_ptr<Location> StructOrUnion::getLocation() const {
    return location;
}

bool StructOrUnion::hasHelperMethods() const { return !fields.empty(); }

Struct::Struct(std::string name, std::vector<std::shared_ptr<Field>> fields,
               uint64_t typeSize, std::shared_ptr<Location> location,
               bool isPacked, bool isBitField)
    : StructOrUnion(std::move(name), std::move(fields), std::move(location)),
      typeSize(typeSize), isPacked(isPacked), hasBitField(isBitField) {}

bool StructOrUnion::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {

    if (contains(this, visitedTypes)) {
        return false;
    }
    visitedTypes.push_back(shared_from_this());

    for (const auto &field : fields) {
        if (*field->getType() == *type ||
            field->getType()->usesType(type, stopOnTypeDefs, visitedTypes)) {
            return true;
        }
    }
    visitedTypes.pop_back();
    return false;
}

std::shared_ptr<TypeDef> Struct::generateTypeDef() {
    if (isRepresentedAsStruct()) {
        return std::make_shared<TypeDef>(getTypeAlias(), shared_from_this(),
                                         nullptr);
    } else {
        // There is no easy way to represent it as a struct in scala native,
        // have to represent it as an array and then Add helpers to help with
        // its manipulation
        return std::make_shared<TypeDef>(
            getTypeAlias(),
            std::make_shared<ArrayType>(std::make_shared<PrimitiveType>("Byte"),
                                        typeSize),
            location);
    }
}

std::string Struct::generateHelperClass() const {
    assert(hasHelperMethods());
    std::stringstream s;
    std::string type = getTypeAlias();
    s << "  implicit class " << type << "_ops(val p: native.Ptr[" << type
      << "])"
      << " extends AnyVal {\n";
    if (isRepresentedAsStruct()) {
        s << generateHelperClassMethodsForStructRepresentation();
    } else {
        s << generateHelperClassMethodsForArrayRepresentation();
    }
    s << "  }\n\n";

    /* makes struct instantiation easier */
    s << "  def "
      << type + "()(implicit z: native.Zone): native.Ptr[" + type + "]"
      << " = native.alloc[" + type + "]\n";

    return s.str();
}

bool Struct::hasHelperMethods() const {
    if (hasBitField) {
        return false;
    }
    if (!isRepresentedAsStruct()) {
        return !fields.empty();
    }
    return !isPacked && !fields.empty();
}

std::string Struct::generateHelperClassMethodsForStructRepresentation() const {
    std::stringstream s;
    for (unsigned fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
        if (!fields[fieldIndex]->getName().empty()) {
            s << generateGetterForStructRepresentation(fieldIndex);
            s << generateSetterForStructRepresentation(fieldIndex);
        }
    }
    return s.str();
}

std::string Struct::generateHelperClassMethodsForArrayRepresentation() const {
    std::stringstream s;
    for (unsigned fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
        if (!fields[fieldIndex]->getName().empty()) {
            s << generateGetterForArrayRepresentation(fieldIndex);
            s << generateSetterForArrayRepresentation(fieldIndex);
        }
    }
    return s.str();
}

std::string Struct::getTypeAlias() const { return "struct_" + name; }

std::string Struct::str() const {
    std::stringstream ss;
    ss << "native.CStruct" << std::to_string(fields.size()) << "[";

    std::string sep = "";
    for (const auto &field : fields) {
        ss << sep;
        std::vector<std::shared_ptr<const Struct>>
            structTypesThatShouldBeReplaced = shouldFieldBreakCycle(field);
        if (structTypesThatShouldBeReplaced.empty()) {
            ss << field->getType()->str();
        } else {
            /* field type is changed to avoid cyclic types in generated code */
            std::shared_ptr<const Type> typeReplacement = getTypeReplacement(
                field->getType(), structTypesThatShouldBeReplaced);
            ss << typeReplacement->str();
        }
        sep = ", ";
    }

    ss << "]";
    return ss.str();
}

bool Struct::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    auto *s = dynamic_cast<const Struct *>(&other);
    if (s) {
        /* structs have unique names */
        return name == s->name;
    }
    return false;
}

std::string
Struct::generateSetterForStructRepresentation(unsigned fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string setter = handleReservedWords(field->getName(), "_=");
    std::string parameterType = field->getType()->str();
    std::string value = "value";
    std::vector<std::shared_ptr<const Struct>> structTypesThatShouldBeReplaced =
        shouldFieldBreakCycle(field);
    if (!structTypesThatShouldBeReplaced.empty()) {
        /* field type is changed to avoid cyclic types in generated code */
        std::shared_ptr<const Type> typeReplacement = getTypeReplacement(
            field->getType(), structTypesThatShouldBeReplaced);
        value = value + ".cast[" + typeReplacement->str() + "]";
    } else if (isAliasForType<ArrayType>(field->getType().get()) ||
               isAliasForType<Struct>(field->getType().get())) {
        parameterType = "native.Ptr[" + parameterType + "]";
        value = "!" + value;
    }
    std::stringstream s;
    s << "    def " << setter << "(value: " + parameterType + "): Unit = !p._"
      << std::to_string(fieldIndex + 1) << " = " << value << "\n";
    return s.str();
}

std::string
Struct::generateGetterForStructRepresentation(unsigned fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string getter = handleReservedWords(field->getName());
    std::string returnType = field->getType()->str();
    std::string methodBody = "p._" + std::to_string(fieldIndex + 1);
    if (isAliasForType<ArrayType>(field->getType().get()) ||
        isAliasForType<Struct>(field->getType().get())) {
        returnType = "native.Ptr[" + returnType + "]";
    } else if (!shouldFieldBreakCycle(field).empty()) {
        /* field type is changed to avoid cyclic types in generated code */
        methodBody =
            "(!" + methodBody + ").cast[" + field->getType()->str() + "]";
    } else {
        methodBody = "!" + methodBody;
    }
    std::stringstream s;
    s << "    def " << getter << ": " << returnType << " = " << methodBody
      << "\n";
    return s.str();
}

bool Struct::isRepresentedAsStruct() const {
    return fields.size() <= SCALA_NATIVE_MAX_STRUCT_FIELDS && !hasBitField;
}

std::string
Struct::generateSetterForArrayRepresentation(unsigned int fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string setter = handleReservedWords(field->getName(), "_=");
    std::string parameterType = field->getType()->str();
    std::string value = "value";
    std::string castedField = "p._1";

    PointerType pointerToFieldType = PointerType(field->getType());
    uint64_t offsetInBytes = field->getOffsetInBits() / 8;
    if (offsetInBytes > 0) {
        castedField =
            "(" + castedField + " + " + std::to_string(offsetInBytes) + ")";
    }
    castedField = "!" + castedField + ".cast[" + pointerToFieldType.str() + "]";
    std::vector<std::shared_ptr<const Struct>> structTypesThatShouldBeReplaced =
        shouldFieldBreakCycle(field);
    if (!structTypesThatShouldBeReplaced.empty()) {
        /* field type is changed to avoid cyclic types in generated code */
        std::shared_ptr<const Type> typeReplacement = getTypeReplacement(
            field->getType(), structTypesThatShouldBeReplaced);
        value = value + ".cast[" + typeReplacement->str() + "]";
    } else if (isAliasForType<ArrayType>(field->getType().get()) ||
               isAliasForType<Struct>(field->getType().get())) {
        parameterType = pointerToFieldType.str();
        value = "!" + value;
    }
    std::stringstream s;
    s << "    def " << setter
      << "(value: " + parameterType + "): Unit = " << castedField << " = "
      << value << "\n";
    return s.str();
}

std::string
Struct::generateGetterForArrayRepresentation(unsigned fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string getter = handleReservedWords(field->getName());
    std::string returnType;
    std::string methodBody;

    PointerType pointerToFieldType = PointerType(field->getType());
    uint64_t offsetInBytes = field->getOffsetInBits() / 8;
    if (offsetInBytes > 0) {
        methodBody = "(p._1 + " + std::to_string(offsetInBytes) + ")";
    } else {
        methodBody = "p._1";
    }
    methodBody = methodBody + ".cast[" + pointerToFieldType.str() + "]";

    if (isAliasForType<ArrayType>(field->getType().get()) ||
        isAliasForType<Struct>(field->getType().get())) {
        returnType = pointerToFieldType.str();
    } else if (!shouldFieldBreakCycle(field).empty()) {
        /* field type is changed to avoid cyclic types in generated code */
        methodBody =
            "(!" + methodBody + ").cast[" + field->getType()->str() + "]";
        returnType = field->getType()->str();
    } else {
        methodBody = "!" + methodBody;
        returnType = field->getType()->str();
    }
    std::stringstream s;
    s << "    def " << getter << ": " << returnType << " = " << methodBody
      << "\n";
    return s.str();
}

std::shared_ptr<const Type>
Struct::getTypeReplacement(std::shared_ptr<const Type> type,
                           std::vector<std::shared_ptr<const Struct>>
                               structTypesThatShouldBeReplaced) const {
    std::shared_ptr<const Type> replacementType = type->unrollTypedefs();
    std::shared_ptr<PointerType> pointerToByte =
        std::make_shared<PointerType>(std::make_shared<PrimitiveType>("Byte"));
    for (const auto &recordType : structTypesThatShouldBeReplaced) {
        std::shared_ptr<TypeDef> recordTypeDef = std::make_shared<TypeDef>(
            recordType->getTypeAlias(), recordType, nullptr);
        std::shared_ptr<Type> pointerToRecord =
            std::make_shared<PointerType>(recordTypeDef);
        if (*replacementType == *pointerToRecord) {
            replacementType = pointerToByte;
        } else {
            replacementType =
                replacementType->replaceType(pointerToRecord, pointerToByte);
        }
        std::vector<std::shared_ptr<const Type>> visitedTypes;
        if (replacementType->usesType(recordType, false, visitedTypes)) {
            assert(isInstanceOf<FunctionPointerType>(replacementType.get()));
            /* function pointer types may have return value or a parameter of
             * value type */
            replacementType = replacementType->replaceType(
                recordTypeDef,
                std::make_shared<PrimitiveType>("native.CStruct0"));
        }
    }
    return replacementType;
}

std::vector<std::shared_ptr<const Struct>>
Struct::shouldFieldBreakCycle(const std::shared_ptr<Field> &field) const {
    std::vector<std::shared_ptr<const Struct>> structTypesThatShouldBeReplaced;
    if (isAliasForType<Struct>(field->getType().get()) ||
        isAliasForType<Union>(field->getType().get())) {
        return structTypesThatShouldBeReplaced;
    }
    CycleNode baseNode(shared_from_base<Struct>(), field.get());
    std::vector<std::shared_ptr<const Type>> visitedTypes;
    if (field->getType()->findAllCycles(shared_from_base<Struct>(), baseNode,
                                        visitedTypes)) {
        /* one or more cycles were found but type of the filed should be
         * changed if this struct has the biggest name compared to other structs
         * in cycle that have fields of non-value type.
         */
        for (const auto &nextCycleNode : baseNode.cycleNodes) {
            std::vector<std::string> namesInCycle;
            if (hasBiggestName(nextCycleNode, namesInCycle)) {
                structTypesThatShouldBeReplaced.push_back(nextCycleNode.s);
            }
        }
    }
    return structTypesThatShouldBeReplaced;
}

bool Struct::findAllCycles(
    const std::shared_ptr<const Struct> &startStruct, CycleNode &cycleNode,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {
    if (this == startStruct.get()) {
        return true;
    }
    /* visitedTypes check is ignored because it is not necessary to save Struct
     * and Union types in visitedTypes if it is done for TypeDefs (Struct and
     * Union types can be references only through TypeDefs) */
    bool belongsToCycle = false;
    for (const auto &field : fields) {
        CycleNode newCycleNode(shared_from_base<Struct>(), field.get());
        if (field->getType()->findAllCycles(startStruct, newCycleNode,
                                            visitedTypes)) {
            if (isAliasForType<Struct>(field->getType().get()) ||
                isAliasForType<Union>(field->getType().get())) {
                /* cycles cannot be broken on value type fields */
                newCycleNode.isValueType = true;
            }
            belongsToCycle = true;
            cycleNode.cycleNodes.push_back(newCycleNode);
        }
    }
    return belongsToCycle;
}

bool Struct::hasBiggestName(const CycleNode &node,
                            std::vector<std::string> namesInCycle) const {
    if (!node.isValueType) {
        namesInCycle.push_back(node.s->getTypeAlias());
    }
    if (node.cycleNodes.empty()) {
        std::sort(namesInCycle.begin(), namesInCycle.end());
        return getTypeAlias() >= namesInCycle.back();
    }
    for (const auto &cycleNode : node.cycleNodes) {
        if (hasBiggestName(cycleNode, namesInCycle)) {
            return true;
        }
    }
    return false;
}

Union::Union(std::string name, std::vector<std::shared_ptr<Field>> fields,
             uint64_t maxSize, std::shared_ptr<Location> location)
    : StructOrUnion(std::move(name), std::move(fields), std::move(location)),
      ArrayType(std::make_shared<PrimitiveType>("Byte"), maxSize) {}

std::shared_ptr<TypeDef> Union::generateTypeDef() {
    return std::make_shared<TypeDef>(getTypeAlias(), shared_from_this(),
                                     nullptr);
}

std::string Union::generateHelperClass() const {
    assert(hasHelperMethods());
    std::stringstream s;
    std::string type = getTypeAlias();
    s << "  implicit class " << type << "_pos"
      << "(val p: native.Ptr[" << type << "]) extends AnyVal {\n";
    for (const auto &field : fields) {
        if (!field->getName().empty()) {
            std::string getter = handleReservedWords(field->getName());
            std::string setter = handleReservedWords(field->getName(), "_=");
            std::shared_ptr<const Type> ftype = field->getType();
            s << "    def " << getter << ": native.Ptr[" << ftype->str()
              << "] = p.cast[native.Ptr[" << ftype->str() << "]]\n";

            s << "    def " << setter << "(value: " << ftype->str()
              << "): Unit = !p.cast[native.Ptr[" << ftype->str()
              << "]] = value\n";
        }
    }
    s << "  }\n";
    return s.str();
}

std::string Union::getTypeAlias() const { return "union_" + name; }

bool Union::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    auto *u = dynamic_cast<const Union *>(&other);
    if (u) {
        /* unions have unique names */
        return name == u->name;
    }
    return false;
}

bool Union::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {

    if (contains(this, visitedTypes)) {
        return false;
    }
    visitedTypes.push_back(shared_from_this());

    if (ArrayType::usesType(type, stopOnTypeDefs, visitedTypes)) {
        return true;
    }
    visitedTypes.pop_back();

    return StructOrUnion::usesType(type, stopOnTypeDefs, visitedTypes);
}
