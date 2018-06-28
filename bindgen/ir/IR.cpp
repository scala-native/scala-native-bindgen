#include "IR.h"
#include "../Utils.h"

IR::IR(std::string libName, std::string linkName, std::string objectName,
       std::string packageName)
    : libName(std::move(libName)), linkName(std::move(linkName)),
      objectName(std::move(objectName)), packageName(std::move(packageName)) {}

void IR::addFunction(std::string name, std::vector<Parameter *> parameters,
                     Type *retType, bool isVariadic) {
    functions.push_back(
        new Function(name, std::move(parameters), retType, isVariadic));
}

void IR::addTypeDef(std::string name, Type *type) {
    typeDefs.push_back(new TypeDef(std::move(name), type));
}

Type *IR::addEnum(std::string name, const std::string &type,
                  std::vector<Enumerator> enumerators) {
    Enum *e = new Enum(std::move(name), type, std::move(enumerators));
    enums.push_back(e);
    if (!e->isAnonymous()) {
        typeDefs.push_back(e->generateTypeDef());
        return typeDefs.back();
    }
    return nullptr;
}

Type *IR::addStruct(std::string name, std::vector<Field *> fields,
                    uint64_t typeSize) {
    Struct *s = new Struct(std::move(name), std::move(fields), typeSize);
    structs.push_back(s);
    typeDefs.push_back(s->generateTypeDef());
    return typeDefs.back();
}

Type *IR::addUnion(std::string name, std::vector<Field *> fields,
                   uint64_t maxSize) {
    Union *u = new Union(std::move(name), std::move(fields), maxSize);
    unions.push_back(u);
    typeDefs.push_back(u->generateTypeDef());
    return typeDefs.back();
}

void IR::addLiteralDefine(std::string name, std::string literal, Type *type) {
    literalDefines.push_back(
        new LiteralDefine(std::move(name), std::move(literal), type));
}

void IR::addPossibleVarDefine(const std::string &macroName,
                              const std::string &varName) {
    possibleVarDefines.push_back(new PossibleVarDefine(macroName, varName));
}

void IR::addVarDefine(std::string name, Variable *variable) {
    varDefines.push_back(new VarDefine(std::move(name), variable));
}

bool IR::libObjEmpty() const {
    return functions.empty() && typeDefs.empty() && structs.empty() &&
           unions.empty() && varDefines.empty();
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir) {
    assert(ir.generated); // typedefs were generated

    if (!ir.packageName.empty()) {
        s << "package " << ir.packageName << "\n\n";
    }

    if (!ir.libObjEmpty() || !ir.enums.empty() || !ir.literalDefines.empty()) {
        s << "import scala.scalanative._\n"
          << "import scala.scalanative.native._\n\n";
    }

    std::string objectName = handleReservedWords(ir.objectName);

    if (!ir.libObjEmpty()) {
        if (!ir.linkName.empty()) {
            s << "@native.link(\"" << ir.linkName << "\")\n";
        }

        s << "@native.extern\n"
          << "object " << objectName << " {\n";

        for (const auto &typeDef : ir.typeDefs) {
            s << *typeDef;
        }

        for (const auto &variable : ir.variables) {
            s << *variable;
        }

        for (const auto &varDefine : ir.varDefines) {
            s << *varDefine;
        }

        for (const auto &func : ir.functions) {
            s << *func;
        }

        s << "}\n\n";
    }

    if (!ir.literalDefines.empty()) {
        s << "object " << ir.libName << "Defines {\n";
        for (const auto &literalDefine : ir.literalDefines) {
            s << *literalDefine;
        }
        s << "}\n\n";
    }

    if (!ir.enums.empty() || ir.hasHelperMethods()) {
        s << "import " << objectName << "._\n\n";
    }

    if (!ir.enums.empty()) {
        s << "object " << ir.libName << "Enums {\n";

        unsigned long enumeratorsCount = ir.enums.size();
        for (unsigned long i = 0; i < enumeratorsCount; i++) {
            auto e = ir.enums[i];
            s << *e;
            if (i < enumeratorsCount - 1) {
                s << "\n"; // space between groups of enums
            }
        }

        s << "}\n\n";
    }

    if (ir.hasHelperMethods()) {
        s << "object " << ir.libName << "Helpers {\n";

        for (const auto &st : ir.structs) {
            s << "\n" << st->generateHelperClass();
        }

        for (const auto &u : ir.unions) {
            s << "\n" << u->generateHelperClass();
        }

        s << "}\n\n";
    }

    return s;
}

void IR::generate(const std::string &excludePrefix) {
    if (!generated) {
        setScalaNames();
        filterDeclarations(excludePrefix);
        generated = true;
    }
}

bool IR::hasHelperMethods() const {
    if (!unions.empty()) {
        /* all unions have helper methods */
        return true;
    }

    for (const auto &s : structs) {
        if (s->hasHelperMethods()) {
            return true;
        }
    }
    return false;
}

void IR::filterDeclarations(const std::string &excludePrefix) {
    if (excludePrefix.empty()) {
        return;
    }

    filterTypeDefs(excludePrefix);

    filterByPrefix(functions, excludePrefix);

    filterByPrefix(literalDefines, excludePrefix);

    filterByPrefix(varDefines, excludePrefix);
}

void IR::filterTypeDefs(const std::string &excludePrefix) {
    for (auto it = typeDefs.begin(); it != typeDefs.end();) {
        TypeDef *typeDef = *it;
        if (startsWith(typeDef->getName(), excludePrefix) &&
            typeIsUsedOnlyInTypeDefs(typeDef)) {
            /* remove this typedef and replace aliases with actual type */
            replaceTypeInTypeDefs(typeDef, typeDef->getType());
            it = typeDefs.erase(it);
        } else {
            ++it;
        }
    }
}

void IR::replaceTypeInTypeDefs(Type *oldType, Type *newType) {
    for (auto &typeDef : typeDefs) {
        if (typeDef->getType() == oldType) {
            typeDef->setType(newType);
        }
    }
}

template <typename T>
bool IR::isTypeUsed(const std::vector<T> &declarations, Type *type) {
    for (const auto decl : declarations) {
        if (decl->usesType(type)) {
            return true;
        }
    }
    return false;
}

bool IR::typeIsUsedOnlyInTypeDefs(Type *type) {
    return !(isTypeUsed(functions, type) || isTypeUsed(structs, type) ||
             isTypeUsed(unions, type));
}

void IR::setScalaNames() {
    /* Renaming according to Scala naming conventions
     * should happen here */

    for (auto &function : functions) {
        if (function->getName() == "native") {
            std::string scalaName = "nativeFunc";
            int i = 0;
            while (existsFunctionWithName(scalaName)) {
                scalaName = "nativeFunc" + std::to_string(i++);
            }
            function->setScalaName(scalaName);
        }
    }
}

bool IR::existsFunctionWithName(std::string functionName) {
    for (const auto &function : functions) {
        if (function->getName() == functionName) {
            return true;
        }
    }
    return false;
}

void IR::removeDefine(const std::string &name) {
    filterByName(literalDefines, name);
    filterByName(possibleVarDefines, name);
    filterByName(varDefines, name);
}

template <typename T>
void IR::filterByPrefix(std::vector<T> &declarations,
                        const std::string &excludePrefix) {
    for (auto it = declarations.begin(); it != declarations.end();) {
        T declaration = *it;
        if (startsWith(declaration->getName(), excludePrefix)) {
            it = declarations.erase(it);
        } else {
            it++;
        }
    }
}

template <typename T>
void IR::filterByName(std::vector<T> &declarations, const std::string &name) {
    for (auto it = declarations.begin(); it != declarations.end();) {
        T declaration = *it;
        if (declaration->getName() == name) {
            it = declarations.erase(it);
        } else {
            it++;
        }
    }
}

std::string IR::getDefineForVar(const std::string &varName) const {
    for (const auto &varDefine : possibleVarDefines) {
        if (varDefine->getVariableName() == varName) {
            return varDefine->getName();
        }
    }
    return "";
}

Variable *IR::addVariable(const std::string &name, Type *type) {
    Variable *variable = new Variable(name, type);
    variables.push_back(variable);
    return variable;
}

TypeDef *IR::getTypeDefWithName(const std::string &name) {
    return getDeclarationWithName(typeDefs, name);
}

template <typename T>
T IR::getDeclarationWithName(std::vector<T> &declarations,
                             const std::string &name) {
    for (auto it = declarations.begin(), end = declarations.end(); it != end;
         ++it) {
        T declaration = (*it);
        if (declaration->getName() == name) {
            return declaration;
        }
    }
    return nullptr;
}

IR::~IR() {
    deallocateTypesThatAreNotInIR(functions);
    deallocateTypesThatAreNotInIR(typeDefs);
    deallocateTypesThatAreNotInIR(structs);
    deallocateTypesThatAreNotInIR(unions);
    deallocateTypesThatAreNotInIR(variables);

    clearVector(functions);
    clearVector(typeDefs);
    clearVector(structs);
    clearVector(unions);
    clearVector(enums);
    clearVector(literalDefines);
    clearVector(possibleVarDefines);
    clearVector(variables);
    clearVector(varDefines);
}

template <typename T> void IR::clearVector(std::vector<T> v) {
    for (const auto &e : v) {
        delete e;
    }
}

template <typename T> void IR::deallocateTypesThatAreNotInIR(std::vector<T> v) {
    for (const auto &e : v) {
        e->deallocateTypesThatAreNotInIR();
    }
}
