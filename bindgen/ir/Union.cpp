#include "Union.h"
#include "../Utils.h"
#include "Struct.h"
#include "types/ArrayType.h"
#include "types/FunctionPointerType.h"
#include "types/PointerType.h"
#include "types/PrimitiveType.h"
#include <sstream>

Union::Union(std::string name, std::vector<std::shared_ptr<Field>> fields,
             uint64_t maxSize, std::shared_ptr<Location> location)
    : Record(std::move(name), std::move(fields), std::move(location)),
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
            s << generateGetter(field);
            s << generateSetter(field);
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
        visitedTypes.pop_back();
        return true;
    }
    visitedTypes.pop_back();

    return Record::usesType(type, stopOnTypeDefs, visitedTypes);
}

std::string Union::generateGetter(const std::shared_ptr<Field> &field) const {
    std::string getter = handleReservedWords(field->getName());
    std::string ftype = field->getType()->str();
    return "    def " + getter + ": native.Ptr[" + ftype +
           "] = p.cast[native.Ptr[" + ftype + "]]\n";
}

std::string Union::generateSetter(const std::shared_ptr<Field> &field) const {
    std::string setter = handleReservedWords(field->getName(), "_=");
    std::string ftype = field->getType()->str();
    if (isAliasForType<ArrayType>(field->getType().get()) ||
        isAliasForType<Struct>(field->getType().get())) {
        return "    def " + setter + "(value: native.Ptr[" + ftype +
               "]): Unit = !p.cast[native.Ptr[" + ftype + "]] = !value\n";
    }
    return "    def " + setter + "(value: " + ftype +
           "): Unit = !p.cast[native.Ptr[" + ftype + "]] = value\n";
}
