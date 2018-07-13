#include "IR.h"
#include "../Utils.h"

IR::IR(std::string libName, std::string linkName, std::string objectName,
       std::string packageName, const LocationManager &locationManager)
    : libName(std::move(libName)), linkName(std::move(linkName)),
      objectName(std::move(objectName)), locationManager(locationManager),
      packageName(std::move(packageName)) {}

void IR::addFunction(std::string name, std::vector<Parameter *> parameters,
                     std::shared_ptr<Type> retType, bool isVariadic) {
    functions.push_back(std::make_shared<Function>(name, std::move(parameters),
                                                   retType, isVariadic));
}

std::shared_ptr<TypeDef> IR::addTypeDef(std::string name,
                                        std::shared_ptr<Type> type,
                                        std::shared_ptr<Location> location) {
    typeDefs.push_back(
        std::make_shared<TypeDef>(std::move(name), type, std::move(location)));
    return typeDefs.back();
}

void IR::addEnum(std::string name, const std::string &type,
                 std::vector<Enumerator> enumerators,
                 std::shared_ptr<Location> location) {
    std::shared_ptr<Enum> e = std::make_shared<Enum>(
        std::move(name), type, std::move(enumerators), std::move(location));
    enums.push_back(e);
    if (!e->isAnonymous()) {
        typeDefs.push_back(e->generateTypeDef());
    }
}

void IR::addStruct(std::string name, std::vector<std::shared_ptr<Field>> fields,
                   uint64_t typeSize, std::shared_ptr<Location> location) {
    std::shared_ptr<Struct> s = std::make_shared<Struct>(
        name, std::move(fields), typeSize, std::move(location));
    structs.push_back(s);
    std::shared_ptr<TypeDef> typeDef = getTypeDefWithName("struct_" + name);
    if (typeDef) {
        /* the struct type used to be opaque type, typeDef contains nullptr */
        typeDef.get()->setType(s);
    } else {
        typeDefs.push_back(s->generateTypeDef());
    }
}

void IR::addUnion(std::string name, std::vector<std::shared_ptr<Field>> fields,
                  uint64_t maxSize, std::shared_ptr<Location> location) {
    std::shared_ptr<Union> u = std::make_shared<Union>(
        name, std::move(fields), maxSize, std::move(location));
    unions.push_back(u);
    std::shared_ptr<TypeDef> typeDef = getTypeDefWithName("union_" + name);
    if (typeDef) {
        /* the union type used to be opaque type, typeDef contains nullptr */
        typeDef.get()->setType(u);
    } else {
        typeDefs.push_back(u->generateTypeDef());
    }
}

void IR::addLiteralDefine(std::string name, std::string literal,
                          std::shared_ptr<Type> type) {
    literalDefines.push_back(std::make_shared<LiteralDefine>(
        std::move(name), std::move(literal), type));
}

void IR::addPossibleVarDefine(const std::string &macroName,
                              const std::string &varName) {
    possibleVarDefines.push_back(
        std::make_shared<PossibleVarDefine>(macroName, varName));
}

void IR::addVarDefine(std::string name, std::shared_ptr<Variable> variable) {
    varDefines.push_back(
        std::make_shared<VarDefine>(std::move(name), variable));
}

bool IR::libObjEmpty() const {
    return functions.empty() && !hasOutputtedDeclaration(typeDefs) &&
           !hasOutputtedDeclaration(structs) &&
           !hasOutputtedDeclaration(unions) && varDefines.empty() &&
           variables.empty();
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir) {
    assert(ir.generated); // typedefs were generated

    if (!ir.packageName.empty()) {
        s << "package " << ir.packageName << "\n\n";
    }

    if (!ir.libObjEmpty() || ir.hasOutputtedDeclaration(ir.enums) ||
        !ir.literalDefines.empty()) {
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
            if (ir.shouldOutput(typeDef)) {
                s << *typeDef;
            }
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

    if (ir.hasOutputtedDeclaration(ir.enums) || ir.hasHelperMethods()) {
        s << "import " << objectName << "._\n\n";
    }

    if (ir.hasOutputtedDeclaration(ir.enums)) {
        s << "object " << ir.libName << "Enums {\n";

        std::string sep = "";
        for (const auto &e : ir.enums) {
            if (ir.shouldOutput(e)) {
                s << sep << *e;
                sep = "\n";
            }
        }

        s << "}\n\n";
    }

    if (ir.hasHelperMethods()) {
        s << "object " << ir.libName << "Helpers {\n";

        for (const auto &st : ir.structs) {
            if (ir.shouldOutput(st) && st->hasHelperMethods()) {
                s << "\n" << st->generateHelperClass();
            }
        }

        for (const auto &u : ir.unions) {
            if (ir.shouldOutput(u)) {
                s << "\n" << u->generateHelperClass();
            }
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
    if (hasOutputtedDeclaration(unions)) {
        /* all unions have helper methods */
        return true;
    }

    for (const auto &s : structs) {
        if (shouldOutput(s) && s->hasHelperMethods()) {
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

    filterByPrefix(variables, excludePrefix);
}

void IR::filterTypeDefs(const std::string &excludePrefix) {
    for (auto it = typeDefs.begin(); it != typeDefs.end();) {
        std::shared_ptr<TypeDef> typeDef = *it;
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

void IR::replaceTypeInTypeDefs(std::shared_ptr<Type> oldType,
                               std::shared_ptr<Type> newType) {
    for (auto &typeDef : typeDefs) {
        if (typeDef->getType() == oldType) {
            typeDef->setType(newType);
        }
    }
}

template <typename T>
bool IR::isTypeUsed(const std::vector<T> &declarations,
                    std::shared_ptr<Type> type, bool stopOnTypeDefs) const {
    for (const auto &decl : declarations) {
        if (decl->usesType(type, stopOnTypeDefs)) {
            return true;
        }
    }
    return false;
}

bool IR::typeIsUsedOnlyInTypeDefs(const std::shared_ptr<Type> &type) const {
    /* varDefines are not checked here because they are simply
     * aliases for variables.*/
    return !(
        isTypeUsed(functions, type, true) || isTypeUsed(structs, type, true) ||
        isTypeUsed(unions, type, true) || isTypeUsed(variables, type, true) ||
        isTypeUsed(literalDefines, type, true));
}

bool IR::isTypeUsed(const std::shared_ptr<Type> &type,
                    bool checkRecursively) const {
    if (checkRecursively) {
        if (isTypeUsed(functions, type, true) ||
            isTypeUsed(variables, type, true) ||
            isTypeUsed(literalDefines, type, true)) {
            return true;
        }
        /* type is used if there exists another type that is used and that
         * references this type */
        for (const auto &typeDef : typeDefs) {
            if (typeDef->usesType(type, false)) {
                if (shouldOutput(typeDef)) {
                    return true;
                }
            }
        }
        for (const auto &s : structs) {
            /* stopOnTypeDefs parameter is true because because typedefs were
             * checked */
            if (s->usesType(type, true)) {
                if (shouldOutput(s)) {
                    return true;
                }
            }
        }
        for (const auto &u : unions) {
            /* stopOnTypeDefs parameter is true because because typedefs were
             * checked */
            if (u->usesType(type, true)) {
                if (shouldOutput(u)) {
                    return true;
                }
            }
        }
        return false;
    } else {
        return !(typeIsUsedOnlyInTypeDefs(type) &&
                 !isTypeUsed(typeDefs, type, false));
    }
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

std::shared_ptr<Variable> IR::addVariable(const std::string &name,
                                          std::shared_ptr<Type> type) {
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(name, type);
    variables.push_back(variable);
    return variable;
}

std::shared_ptr<TypeDef> IR::getTypeDefWithName(const std::string &name) const {
    /* nullptr is returned in 2 cases:
     * 1. TypeTranslator translates opaque struct/union type for which TypeDef
     *    was not created.
     * 2. TreeVisitor visits struct/union declaration and it checks whether a
     *    TypeDef already exists for it.*/
    return getDeclarationWithName(typeDefs, name);
}

template <typename T>
T IR::getDeclarationWithName(const std::vector<T> &declarations,
                             const std::string &name) const {
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
    functions.clear();
    typeDefs.clear();
    structs.clear();
    unions.clear();
    enums.clear();
    literalDefines.clear();
    possibleVarDefines.clear();
    variables.clear();
    varDefines.clear();
}

template <typename T> bool IR::inMainFile(const T &type) const {
    std::shared_ptr<Location> location = type.getLocation();
    if (!location) {
        /* generated TypeDef */
        auto *typeDef = dynamic_cast<const TypeDef *>(&type);
        assert(typeDef);
        Type *innerType = typeDef->getType().get();
        if (isInstanceOf<Struct>(innerType)) {
            return inMainFile(*dynamic_cast<Struct *>(innerType));
        }
        if (isInstanceOf<Union>(innerType)) {
            return inMainFile(*dynamic_cast<Union *>(innerType));
        }
        if (isInstanceOf<Enum>(innerType)) {
            return inMainFile(*dynamic_cast<Enum *>(innerType));
        }
    }
    return location && locationManager.inMainFile(*location);
}

template <typename T>
bool IR::hasOutputtedDeclaration(
    const std::vector<std::shared_ptr<T>> &declarations) const {
    for (const auto &declaration : declarations) {
        if (shouldOutput(declaration)) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool IR::shouldOutput(const std::shared_ptr<T> &type) const {
    return inMainFile(*type) || isTypeUsed(type, true);
}
