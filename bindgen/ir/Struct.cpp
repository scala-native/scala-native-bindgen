#include "Struct.h"
#include "../Utils.h"
#include "types/ArrayType.h"
#include "types/PointerType.h"
#include "types/PrimitiveType.h"
#include <sstream>

Field::Field(std::string name, std::shared_ptr<Type> type)
    : TypeAndName(std::move(name), std::move(type)) {}

Field::Field(std::string name, std::shared_ptr<Type> type, uint64_t offset)
    : TypeAndName(std::move(name), std::move(type)), offset(offset) {}

uint64_t Field::getOffset() const { return offset; }

StructOrUnion::StructOrUnion(std::string name,
                             std::vector<std::shared_ptr<Field>> fields,
                             std::shared_ptr<Location> location)
    : name(std::move(name)), fields(std::move(fields)),
      location(std::move(location)) {}

std::string StructOrUnion::getName() const { return name; }

bool StructOrUnion::equals(const StructOrUnion &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<const Struct>(&other)) {
        auto *s = dynamic_cast<const Struct *>(&other);
        if (name != s->name) {
            return false;
        }
        if (fields.size() != s->fields.size()) {
            return false;
        }
        for (size_t i = 0; i < fields.size(); i++) {
            if (*fields[i] != *s->fields[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::shared_ptr<Location> StructOrUnion::getLocation() const {
    return location;
}

bool StructOrUnion::hasHelperMethods() const { return !fields.empty(); }

Struct::Struct(std::string name, std::vector<std::shared_ptr<Field>> fields,
               uint64_t typeSize, std::shared_ptr<Location> location)
    : StructOrUnion(std::move(name), std::move(fields), std::move(location)),
      typeSize(typeSize) {}

std::shared_ptr<TypeDef> Struct::generateTypeDef() {
    if (fields.size() < SCALA_NATIVE_MAX_STRUCT_FIELDS) {
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
    if (fields.size() <= SCALA_NATIVE_MAX_STRUCT_FIELDS) {
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
        ss << sep << field->getType()->str();
        sep = ", ";
    }

    ss << "]";
    return ss.str();
}

bool Struct::usesType(const std::shared_ptr<Type> &type,
                      bool stopOnTypeDefs) const {
    for (const auto &field : fields) {
        if (*field->getType() == *type ||
            field->getType().get()->usesType(type, stopOnTypeDefs)) {
            return true;
        }
    }
    return false;
}

bool Struct::operator==(const Type &other) const {
    auto *s = dynamic_cast<const Struct *>(&other);
    if (s) {
        return this->equals(*s);
    }
    return false;
}

std::string
Struct::generateSetterForStructRepresentation(unsigned fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string setter = handleReservedWords(field->getName(), "_=");
    std::string parameterType = field->getType()->str();
    std::string value = "value";
    if (isAliasForType<ArrayType>(field->getType().get()) ||
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
    std::string methodBody;
    if (isAliasForType<ArrayType>(field->getType().get()) ||
        isAliasForType<Struct>(field->getType().get())) {
        returnType = "native.Ptr[" + returnType + "]";
        methodBody = "p._" + std::to_string(fieldIndex + 1);
    } else {
        methodBody = "!p._" + std::to_string(fieldIndex + 1);
    }
    std::stringstream s;
    s << "    def " << getter << ": " << returnType << " = " << methodBody
      << "\n";
    return s.str();
}

std::string
Struct::generateSetterForArrayRepresentation(unsigned int fieldIndex) const {
    return std::string();
}

std::string
Struct::generateGetterForArrayRepresentation(unsigned fieldIndex) const {
    std::shared_ptr<Field> field = fields[fieldIndex];
    std::string getter = handleReservedWords(field->getName());
    std::string returnType;
    std::string methodBody;

    PointerType pointerToFieldType = PointerType(field->getType());
    if (field->getOffset() != 0) {
        methodBody = "(p._1 + " + std::to_string(field->getOffset()) + ")";
    } else {
        methodBody = "p._1";
    }
    methodBody = methodBody + ".cast[" + pointerToFieldType.str() + "]";

    if (isAliasForType<ArrayType>(field->getType().get()) ||
        isAliasForType<Struct>(field->getType().get())) {
        returnType = pointerToFieldType.str();
    } else {
        methodBody = "!" + methodBody;
        returnType = field->getType()->str();
    }
    std::stringstream s;
    s << "    def " << getter << ": " << returnType << " = " << methodBody
      << "\n";
    return s.str();
    return "";
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
            std::shared_ptr<Type> ftype = field->getType();
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
    auto *u = dynamic_cast<const Union *>(&other);
    if (u) {
        return this->equals(*u);
    }
    return false;
}
