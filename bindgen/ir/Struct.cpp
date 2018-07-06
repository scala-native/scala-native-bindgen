#include "Struct.h"
#include "../Utils.h"
#include "types/ArrayType.h"
#include "types/PrimitiveType.h"
#include <sstream>

Field::Field(std::string name, std::shared_ptr<Type> type)
    : TypeAndName(std::move(name), std::move(type)) {}

std::string Field::generateSetter(int fieldIndex) {
    std::string setter = handleReservedWords(getName(), "_=");
    std::string parameterType = type->str();
    std::string value = "value";
    if (isAliasForType<ArrayType>(type.get()) ||
        isAliasForType<Struct>(type.get())) {
        parameterType = "native.Ptr[" + parameterType + "]";
        value = "!" + value;
    }
    std::stringstream s;
    s << "    def " << setter << "(value: " + parameterType + "): Unit = !p._"
      << std::to_string(fieldIndex + 1) << " = " << value;
    return s.str();
}

std::string Field::generateGetter(int fieldIndex) {
    std::string getter = handleReservedWords(getName());
    std::string returnType = type->str();
    std::string methodBody;
    if (isAliasForType<ArrayType>(type.get()) ||
        isAliasForType<Struct>(type.get())) {
        returnType = "native.Ptr[" + returnType + "]";
        methodBody = "p._" + std::to_string(fieldIndex + 1);
    } else {
        methodBody = "!p._" + std::to_string(fieldIndex + 1);
    }
    std::stringstream s;
    s << "    def " << getter << ": " << returnType << " = " << methodBody;
    return s.str();
}

StructOrUnion::StructOrUnion(std::string name, std::vector<Field *> fields)
    : name(std::move(name)), fields(std::move(fields)) {}

std::string StructOrUnion::getName() const { return name; }

StructOrUnion::~StructOrUnion() {
    for (const auto &field : fields) {
        delete field;
    }
}

bool StructOrUnion::operator==(const StructOrUnion &other) const {
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

Struct::Struct(std::string name, std::vector<Field *> fields, uint64_t typeSize)
    : StructOrUnion(std::move(name), std::move(fields)), typeSize(typeSize) {}

std::shared_ptr<TypeDef> Struct::generateTypeDef() {
    if (fields.size() < SCALA_NATIVE_MAX_STRUCT_FIELDS) {
        return std::make_shared<TypeDef>(getAliasType(), shared_from_this());
    } else {
        // There is no easy way to represent it as a struct in scala native,
        // have to represent it as an array and then Add helpers to help with
        // its manipulation
        return std::make_shared<TypeDef>(
            getAliasType(),
            std::make_shared<ArrayType>(std::make_shared<PrimitiveType>("Byte"),
                                        typeSize));
    }
}

std::string Struct::generateHelperClass() const {
    assert(hasHelperMethods());
    /* struct is not empty and not represented as an array */
    std::stringstream s;
    std::string type = getAliasType();
    s << "  implicit class " << type << "_ops(val p: native.Ptr[" << type
      << "])"
      << " extends AnyVal {\n";
    int fieldIndex = 0;
    for (const auto &field : fields) {
        if (!field->getName().empty()) {
            s << field->generateGetter(fieldIndex) << "\n";
            s << field->generateSetter(fieldIndex) << "\n";
        }
        fieldIndex++;
    }
    s << "  }\n\n";

    /* makes struct instantiation easier */
    s << "  def "
      << type + "()(implicit z: native.Zone): native.Ptr[" + type + "]"
      << " = native.alloc[" + type + "]\n";

    return s.str();
}

bool Struct::hasHelperMethods() const {
    return !fields.empty() && fields.size() < SCALA_NATIVE_MAX_STRUCT_FIELDS;
}

std::string Struct::getAliasType() const { return "struct_" + name; }

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

bool Struct::usesType(const std::shared_ptr<Type> &type) const {
    for (const auto &field : fields) {
        if (*field->getType() == *type) {
            return true;
        }
    }
    return false;
}

Union::Union(std::string name, std::vector<Field *> fields, uint64_t maxSize)
    : StructOrUnion(std::move(name), std::move(fields)),
      ArrayType(std::make_shared<PrimitiveType>("Byte"), maxSize) {}

std::shared_ptr<TypeDef> Union::generateTypeDef() {
    return std::make_shared<TypeDef>(getTypeAlias(), shared_from_this());
}

std::string Union::generateHelperClass() const {
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
