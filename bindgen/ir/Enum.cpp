#include "Enum.h"

Enumerator::Enumerator(std::string name, int64_t value)
    : name(std::move(name)), value(value) {}

std::string Enumerator::getName() { return name; }

int64_t Enumerator::getValue() { return value; }

Enum::Enum(std::string name, std::string type,
           std::vector<Enumerator> enumerators,
           std::shared_ptr<Location> location)
    : PrimitiveType(std::move(type)), name(std::move(name)),
      enumerators(std::move(enumerators)), location(std::move(location)) {}

bool Enum::isAnonymous() const { return name.empty(); }

std::shared_ptr<TypeDef> Enum::generateTypeDef() {
    return std::make_shared<TypeDef>(getTypeAlias(), shared_from_this(),
                                     nullptr);
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e) {
    for (auto enumerator : e.enumerators) {
        std::string enumeratorName;
        if (!e.name.empty()) {
            enumeratorName = "enum_" + e.name + "_" + enumerator.getName();
        } else {
            enumeratorName = "enum_" + enumerator.getName();
        }
        s << "  final val " << enumeratorName;
        std::string type;
        if (e.isAnonymous()) {
            type = e.getType();
        } else {
            type = "enum_" + e.name;
        }
        s << ": " << type << " = " << std::to_string(enumerator.getValue());

        if (e.getType() == "native.CLong") {
            s << "L";
        } else if (e.getType() == "native.CUnsignedInt") {
            s << ".toUInt";
        } else if (e.getType() == "native.CUnsignedLong") {
            s << "L.toULong";
        }
        s << "\n";
    }
    return s;
}

std::string Enum::getName() const { return name; }

std::string Enum::getTypeAlias() const {
    assert(!isAnonymous());
    return "enum_" + name;
}

std::shared_ptr<Location> Enum::getLocation() { return location; }
