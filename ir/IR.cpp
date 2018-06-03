#include <sstream>
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
        s << "import scala.scalanative._" << std::endl
          << "import scala.scalanative.native._" << std::endl
          << "import scala.scalanative.native.Nat._" << std::endl << std::endl;
    }

    std::string libObjName = handleReservedWords(libName);

    if (!libObjEmpty()) {
        s << "@native.link(\"" << libName << "\")" << std::endl
          << "@native.extern" << std::endl
          << "object " << libObjName << " {" << std::endl;

        for (const auto &typeDef : typeDefs) {
            s << typeDef;
        }

        for (const auto &func : functions) {
            s << func;
        }

        s << "}" << std::endl << std::endl;
    }

    if (!enums.empty() || hasHelperMethods()) {
        s << "import " << libObjName << "._" << std::endl << std::endl;
    }

    if (!enums.empty()) {
        s << "object " << libName << "Enums {" << std::endl;

        for (const auto &e : enums) {
            s << e
              << std::endl; // space between groups of enums
        }

        s << "}" << std::endl << std::endl;
    }

    if (hasHelperMethods()) {
        s << "object " << libName << "Helpers {" << std::endl;

        for (const auto &st : structs) {
            s << std::endl
              << st.generateHelperClass();
        }

        for (const auto &u : unions) {
            s << std::endl
              << u.generateHelperClass();
        }

        s << "}" << std::endl << std::endl;
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

bool IR::hasEnums() const {
    return enums.size() != 0;
}
