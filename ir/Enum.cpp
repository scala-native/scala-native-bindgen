#include "Enum.h"
#include <sstream>
#include <cassert>

Enumerator::Enumerator(std::string name, uint64_t value)
        : name(std::move(name)), value(value) {}

std::string Enumerator::getName() {
    return name;
}

uint64_t Enumerator::getValue() {
    return value;
}

Enum::Enum(std::string name, std::string type, std::vector<Enumerator> enumerators)
        : name(std::move(name)), type(std::move(type)), enumerators(std::move(enumerators)) {}

bool Enum::isAnonymous() const {
    return name.empty();
}

TypeDef Enum::generateTypeDef() const {
    assert (!isAnonymous());
    return TypeDef("enum_" + name, type);
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Enum &e) {
    for (auto enumerator : e.enumerators) {
        std::string enumeratorName;
        if (!e.name.empty()) {
            enumeratorName = "enum_" + e.name + "_" + enumerator.getName();
        } else {
            enumeratorName = "enum_" + enumerator.getName();
        }
        s << "  final val " << enumeratorName << " = " << std::to_string(enumerator.getValue()) << "\n";
    }
    return s;
}
