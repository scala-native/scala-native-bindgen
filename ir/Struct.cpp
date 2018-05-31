#include "Struct.h"
#include "../Utils.h"
#include <utility>
#include <sstream>

Field::Field(std::string name, std::string type)
        : TypeAndName(std::move(name), std::move(type)) {}

StructOrUnion::StructOrUnion(std::string name, std::vector<Field> fields)
        : name(std::move(name)), fields(std::move(fields)) {}

Struct::Struct(std::string name, std::vector<Field> fields, uint64_t typeSize)
        : StructOrUnion(std::move(name), std::move(fields)), typeSize(typeSize) {}

TypeDef Struct::generateTypeDef() {
    if (fields.size() < SCALA_NATIVE_MAX_STRUCT_FIELDS) {
        return TypeDef("struct_" + name,
                       "native.CStruct" + std::to_string(fields.size()) + "[" + getFieldsTypes() + "]");
    } else {
        // There is no easy way to represent it as a struct in scala native, have to represent it as an array and then
        // Add helpers to help with it's manipulation
        return TypeDef("struct_" + name, "native.CArray[Byte, " + uint64ToScalaNat(typeSize) + "]");
    }
}

std::string Struct::getFieldsTypes() {
    std::stringstream s;
    auto fieldsCount = static_cast<int>(fields.size());
    for (int i = 0; i < fieldsCount; i++) {
        s << fields[i].getType();
        if (i < fieldsCount - 1) {
            s << ", ";
        }
    }
    return s.str();
}

std::string Struct::generateHelperClass() const {
    if (fields.empty() || fields.size() >= SCALA_NATIVE_MAX_STRUCT_FIELDS) {
        /* struct is represented as an array */
        return "";
    }
    std::stringstream s;
    std::string newName = "struct_" + name;
    s << "  implicit class " << newName << "_ops(val p: native.Ptr[struct_" << name << "]) extends AnyVal {\n";
    int fieldIndex = 0;
    for (const auto &field : fields) {
        if (!field.getName().empty()) {
            std::string fname = handleReservedWords(field.getName());
            std::string ftype = field.getType();
            s << "    def " << fname << ": " << ftype << " = !p._" << std::to_string(fieldIndex + 1) << "\n";
            s << "    def " << fname << "_=(value: " + ftype + "):Unit = !p._" << std::to_string(fieldIndex + 1)
              << " = value\n";
        }
        fieldIndex++;
    }
    s << "  }\n\n";

    /* makes struct instantiation easier */
    s << "  def " << newName + "()(implicit z: native.Zone): native.Ptr[" + newName + "] = native.alloc[" + newName + "]\n";

    return s.str();
}

Union::Union(std::string name,
             std::vector<Field> members, uint64_t maxSize)
        : StructOrUnion(std::move(name), std::move(members)), maxSize(maxSize) {}

TypeDef Union::generateTypeDef() {
    return TypeDef("union_" + name, "native.CArray[Byte, " + uint64ToScalaNat(maxSize) + "]");
}

std::string Union::generateHelperClass() const {
    std::stringstream s;
    s << "  implicit class union_" << name << "_pos"
      << "(val p: native.Ptr[native.CArray[Byte, " << uint64ToScalaNat(maxSize) << "]]) extends AnyVal {\n";
    for (const auto &field : fields) {
        if (!field.getName().empty()) {
            std::string fname = handleReservedWords(field.getName());
            std::string ftype = field.getType();
            s << "    def " << fname
              << ": native.Ptr[" << ftype << "] = p.cast[native.Ptr[" << ftype << "]]\n";

            s << "    def " << fname
              << "_=(value: " << ftype << "): Unit = !(p.cast[native.Ptr[" << ftype << "]]) = value\n";
        }
    }
    s << " }\n";
    return s.str();
}
