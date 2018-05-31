#include <sstream>
#include <utility>
#include <llvm/Support/raw_ostream.h>
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

std::string IR::generate() {
    generateDecl();
    std::stringstream s;
    if (!libObjEmpty() || !enums.empty()) {
        s << "import scala.scalanative._\n"
          << "import scala.scalanative.native._\n"
          << "import scala.scalanative.native.Nat._\n\n";
    }

    std::string libObjName = handleReservedWords(libName);

    if (!libObjEmpty()) {
        s << "@native.link(\"" << libName << "\")\n"
          << "@native.extern\n"
          << "object " << libObjName << " {\n";

        for (const auto &typeDef : typeDefs) {
            s << typeDef;
        }

        for (const auto &func : functions) {
            s << func;
        }

        s << "}\n\n";
    }

    if (!enums.empty()) {
        llvm::outs() << "import " + libObjName + "._\n\n";

        s << "object " << libName << "Enums {\n";

        for (const auto &e : enums) {
            s << e
              << "\n"; // space between groups of enums
        }

        s << "}\n\n";
    }

    if (hasHelperMethods()) {
        s << "object " << libName << "Helpers {\n";

        for (const auto &st : structs) {
            s << "\n";
            s << st.generateHelperClass();
        }

        for (const auto &u : unions) {
            s << "\n";
            s << u.generateHelperClass();
        }

        s << "}\n";
    }

    return s.str();
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

void IR::generateDecl() {
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
