#include "IR.h"
#include "../Utils.h"

IR::IR(std::string libName) : libName(std::move(libName)) {
    if (this->libName == "native") {
        /* there are at most 3 objects in the file.
         * All of them will have distinct names. */
        libObjectName = "nativeLib";
    } else {
        libObjectName = this->libName;
    }
}

void IR::addFunction(std::string name, std::vector<Parameter> parameters,
                     std::string retType, bool isVariadic) {
    functions.emplace_back(std::move(name), std::move(parameters),
                           std::move(retType), isVariadic);
}

void IR::addTypeDef(std::string name, std::string type) {
    typeDefs.emplace_back(std::move(name), std::move(type));
}

void IR::addEnum(std::string name, std::string type,
                 std::vector<Enumerator> enumerators) {
    enums.emplace_back(std::move(name), std::move(type),
                       std::move(enumerators));
}

void IR::addStruct(std::string name, std::vector<Field> fields,
                   uint64_t typeSize) {
    structs.emplace_back(std::move(name), std::move(fields), typeSize);
}

void IR::addUnion(std::string name, std::vector<Field> fields,
                  uint64_t maxSize) {
    unions.emplace_back(std::move(name), std::move(fields), maxSize);
}

bool IR::libObjEmpty() const {
    return functions.empty() && typeDefs.empty() && structs.empty() &&
           unions.empty();
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir) {
    assert(ir.generated); // typedefs were generated

    if (!ir.packageName.empty()) {
        s << "package " << ir.packageName << "\n\n";
    }

    if (!ir.libObjEmpty() || !ir.enums.empty()) {
        s << "import scala.scalanative._\n"
          << "import scala.scalanative.native._\n\n";
    }

    std::string libObjName = handleReservedWords(ir.libObjectName);

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
            s << "\n" << st.generateHelperClass();
        }

        for (const auto &u : ir.unions) {
            s << "\n" << u.generateHelperClass();
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

void IR::generate(const std::string &excludePrefix) {
    if (!generated) {
        setScalaNames();
        filterDeclarations(excludePrefix);
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

bool IR::hasEnums() const { return !enums.empty(); }

void IR::filterDeclarations(const std::string &excludePrefix) {
    if (excludePrefix.empty()) {
        return;
    }

    filterTypeDefs(excludePrefix);

    filterFunctions(excludePrefix);
}

void IR::filterTypeDefs(const std::string &excludePrefix) {
    for (auto it = typeDefs.begin(); it != typeDefs.end();) {
        TypeDef &typeDef = *it;
        if (startsWith(typeDef.getName(), excludePrefix) &&
            typeIsUsedOnlyInTypeDefs(typeDef.getName())) {
            /* remove this typedef and replace aliases with actual type */
            replaceTypeInTypeDefs(typeDef.getName(), typeDef.getType());
            it = typeDefs.erase(it);
        } else {
            ++it;
        }
    }
}

void IR::replaceTypeInTypeDefs(const std::string &oldType,
                               const std::string &newType) {
    for (auto &typeDef : typeDefs) {
        if (typeDef.getType() == oldType) {
            typeDef.setType(newType);
        }
    }
}

void IR::filterFunctions(const std::string &excludePrefix) {
    for (auto it = functions.begin(); it != functions.end();) {
        Function &function = *it;
        if (startsWith(function.getName(), excludePrefix)) {
            it = functions.erase(it);
        } else {
            it++;
        }
    }
}

template <typename T>
bool IR::isTypeUsed(const std::vector<T> &declarations,
                    const std::string &type) {
    for (const auto &decl : declarations) {
        if (decl.usesType(type)) {
            return true;
        }
    }
    return false;
}

bool IR::typeIsUsedOnlyInTypeDefs(std::string type) {
    return !(isTypeUsed(functions, type) || isTypeUsed(structs, type) ||
             isTypeUsed(unions, type));
}

void IR::setPackageName(std::string packageName) {
    this->packageName = std::move(packageName);
}

void IR::setScalaNames() {
    /* Renaming according to Scala naming conventions
     * should happen here */

    for (auto &function : functions) {
        if (function.getName() == "native") {
            std::string scalaName = "nativeFunc";
            int i = 0;
            while (existsFunctionWithName(scalaName)) {
                scalaName = "nativeFunc" + std::to_string(i++);
            }
            function.setScalaName(scalaName);
        }
    }
}

bool IR::existsFunctionWithName(std::string functionName) {
    for (const auto &function : functions) {
        if (function.getName() == functionName) {
            return true;
        }
    }
    return false;
}
