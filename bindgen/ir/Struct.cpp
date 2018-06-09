#include "Struct.h"
#include "../Utils.h"
#include <sstream>

Field::Field(std::string name, std::string type)
    : TypeAndName(std::move(name), std::move(type)) {}

StructOrUnion::StructOrUnion(std::string name, std::vector<Field> fields)
    : name(std::move(name)), fields(std::move(fields)) {}

std::string StructOrUnion::getName() const { return name; }

bool StructOrUnion::usesType(const std::string &type) const {
    for (const auto &field : fields) {
        if (typeUsesOtherType(field.getType(), type)) {
            return true;
        }
    }
    return false;
}

Struct::Struct(std::string name, std::vector<Field> fields, uint64_t typeSize)
    : StructOrUnion(std::move(name), std::move(fields)), typeSize(typeSize) {}

TypeDef Struct::generateTypeDef() const {
    if (fields.size() < SCALA_NATIVE_MAX_STRUCT_FIELDS) {
        return TypeDef("struct_" + name, "native.CStruct" +
                                             std::to_string(fields.size()) +
                                             "[" + getFieldsTypes() + "]");
    } else {
        // There is no easy way to represent it as a struct in scala native,
        // have to represent it as an array and then Add helpers to help with
        // its manipulation
        return TypeDef("struct_" + name, "native.CArray[Byte, " +
                                             uint64ToScalaNat(typeSize) + "]");
    }
}

std::string Struct::getFieldsTypes() const {
    std::stringstream s;
    std::string sep = "";
    for (const auto &field : fields) {
        s << sep << field.getType();
        sep = ", ";
    }
    return s.str();
}

std::string Struct::generateHelperClass() const {
    if (!hasHelperMethods()) {
        /* struct is empty or represented as an array */
        return "";
    }
    std::stringstream s;
    std::string type = getType();
    s << "  implicit class " << type << "_ops(val p: native.Ptr[" << type
      << "])"
      << " extends AnyVal {\n";
    int fieldIndex = 0;
    for (const auto &field : fields) {
        if (!field.getName().empty()) {
            std::string getter = handleReservedWords(field.getName());
            std::string setter = handleReservedWords(field.getName(), "_=");
            std::string ftype = field.getType();
            s << "    def " << getter << ": " << ftype << " = !p._"
              << std::to_string(fieldIndex + 1) << "\n"
              << "    def " << setter << "(value: " + ftype + "):Unit = !p._"
              << std::to_string(fieldIndex + 1) << " = value\n";
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

std::string Struct::getType() const { return "struct_" + name; }

Union::Union(std::string name, std::vector<Field> members, uint64_t maxSize)
    : StructOrUnion(std::move(name), std::move(members)), maxSize(maxSize) {}

TypeDef Union::generateTypeDef() const {
    return TypeDef(getType(),
                   "native.CArray[Byte, " + uint64ToScalaNat(maxSize) + "]");
}

std::string Union::generateHelperClass() const {
    std::stringstream s;
    std::string type = getType();
    s << "  implicit class " << type << "_pos"
      << "(val p: native.Ptr[" << type << "]) extends AnyVal {\n";
    for (const auto &field : fields) {
        if (!field.getName().empty()) {
            std::string getter = handleReservedWords(field.getName());
            std::string setter = handleReservedWords(field.getName(), "_=");
            std::string ftype = field.getType();
            s << "    def " << getter << ": native.Ptr[" << ftype
              << "] = p.cast[native.Ptr[" << ftype << "]]\n";

            s << "    def " << setter << "(value: " << ftype
              << "): Unit = !p.cast[native.Ptr[" << ftype << "]] = value\n";
        }
    }
    s << "  }\n";
    return s.str();
}

std::string Union::getType() const { return "union_" + name; }
