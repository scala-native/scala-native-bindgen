#include "Enum.h"
#include <sstream>
#include <cassert>

Enumerator::Enumerator(std::string name, uint64_t value)
        : name(std::move(name)), value(value) {}

std::string Enumerator::getName() {
    return name;
}

Enum::Enum(std::string name, std::vector<Enumerator> enumerators)
        : name(std::move(name)), enumerators(std::move(enumerators)) {}

bool Enum::isAnonymous() const {
    return name.empty();
}

TypeDef Enum::generateTypeDef() const {
    assert (!isAnonymous());
    return TypeDef("enum_" + name, "native.CInt");
}

std::ostream &operator<<(std::ostream &s, const Enum &e) {
    int i = 0;
    for (auto enumerator : e.enumerators) {
        std::string enumeratorName;
        if (!e.name.empty()) {
            enumeratorName = "enum_" + e.name + "_" + enumerator.getName();
        } else {
            enumeratorName = "enum_" + enumerator.getName();
        }
        s << "  final val " << enumeratorName << " = " << std::to_string(i++) << std::endl;
    }
    return s;
}
