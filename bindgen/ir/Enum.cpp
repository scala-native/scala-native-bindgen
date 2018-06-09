#include "Enum.h"
#include <sstream>

Enumerator::Enumerator(std::string name, int64_t value)
    : name(std::move(name)), value(value) {}

std::string Enumerator::getName() { return name; }

int64_t Enumerator::getValue() { return value; }

Enum::Enum(std::string name, std::string type,
           std::vector<Enumerator> enumerators)
    : name(std::move(name)), type(std::move(type)),
      enumerators(std::move(enumerators)) {}

bool Enum::isAnonymous() const { return name.empty(); }

TypeDef Enum::generateTypeDef() const {
    assert(!isAnonymous());
    return TypeDef(getTypeName(), type);
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e) {
    for (auto enumerator : e.enumerators) {
        std::string enumeratorName;
        if (!e.name.empty()) {
            enumeratorName = e.getTypeName() + "_" + enumerator.getName();
        } else {
            enumeratorName = "enum_" + enumerator.getName();
        }
        s << "  final val " << enumeratorName;
        std::string type;
        if (e.isAnonymous()) {
            type = e.type;
        } else {
            type = e.getTypeName();
        }
        s << ": " << type << " = " << std::to_string(enumerator.getValue());

        if (e.type == "native.CLong") {
            s << "L";
        } else if (e.type == "native.CUnsignedInt") {
            s << ".toUInt";
        } else if (e.type == "native.CUnsignedLong") {
            s << "L.toULong";
        }
        s << "\n";
    }
    return s;
}

std::string Enum::getTypeName() const {
    assert(!isAnonymous());
    return "enum_" + name;
}
