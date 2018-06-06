#include "IR.h"
#include "../Utils.h"


IR::IR(std::string libName) :
        libName(std::move(libName)) {}


void IR::addFunction(std::string name,
                     std::vector<Parameter> parameters,
                     std::string retType, bool isVariadic) {
    functions.push_back(Function(std::move(name), std::move(parameters),
                                 std::move(retType), isVariadic));
}

void IR::addTypeDef(std::string name, std::string type) {
    typeDefs.push_back(TypeDef(std::move(name), std::move(type)));
}

void IR::addEnum(std::string name, std::vector<Enumerator> enumerators) {
    enums.push_back(Enum(std::move(name), std::move(enumerators)));
}

void IR::addStruct(std::string name, std::vector<Field> fields, uint64_t typeSize) {
    structs.push_back(Struct(std::move(name), std::move(fields), typeSize));
}

void IR::addUnion(std::string name, std::vector<Field> fields, uint64_t maxSize) {
    unions.push_back(Union(std::move(name), std::move(fields), maxSize));
}

bool IR::libObjEmpty() const {
    return functions.empty() && typeDefs.empty() &&
           structs.empty() && unions.empty();
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir) {
    assert(ir.generated); // typedefs were generated

    if (!ir.libObjEmpty() || !ir.enums.empty()) {
        s << "import scala.scalanative._\n"
          << "import scala.scalanative.native._\n"
          << "import scala.scalanative.native.Nat._\n\n";
    }

    std::string libObjName = handleReservedWords(ir.libName);

    if (!ir.libObjEmpty()) {
        s << "@native.link(\"" << ir.libName << "\")\n"
          << "@native.extern\n"
          << "object " << libObjName << " {\n";

        for (const auto &typeDef : ir.typeDefs) {
            s << typeDef;
        }

        for (const auto &func : ir.functions) {
            s << func;
        }

        s << "}\n\n";
    }

    if (!ir.enums.empty() || ir.hasHelperMethods()) {
        s << "import " << libObjName << "._\n\n";
    }

    if (!ir.enums.empty()) {
        s << "object " << ir.libName << "Enums {\n";

        unsigned long enumeratorsCount = ir.enums.size();
        for (unsigned long i = 0; i < enumeratorsCount; i++) {
            auto &e = ir.enums[i];
            s << e;
            if (i < enumeratorsCount - 1) {
                s << "\n"; // space between groups of enums
            }
        }

        s << "}\n\n";
    }

    if (ir.hasHelperMethods()) {
        s << "object " << ir.libName << "Helpers {\n";

        for (const auto &st : ir.structs) {
            s << "\n"
              << st.generateHelperClass();
        }

        for (const auto &u : ir.unions) {
            s << "\n"
              << u.generateHelperClass();
        }

        s << "}\n\n";
    }

    return s;
}

void IR::generateTypeDefs() {
    for (const auto &e : enums) {
        if (!e.isAnonymous()) { // enum might be anon
            typeDefs.push_back(e.generateTypeDef());
        }
    }
    for (const auto &s : structs) {
        typeDefs.push_back(s.generateTypeDef());
    }
    for (const auto &u : unions) {
        typeDefs.push_back(u.generateTypeDef());
    }
}

void IR::generate() {
    if (!generated) {
        generateTypeDefs();
        generated = true;
    }
}

bool IR::hasHelperMethods() const {
    if (!unions.empty()) {
        /* all unions have helper methods */
        return true;
    }

    for (const auto &s : structs) {
        if (s.hasHelperMethods()) {
            return true;
        }
    }
    return false;
}

bool IR::hasEnums() const {
    return !enums.empty();
}
