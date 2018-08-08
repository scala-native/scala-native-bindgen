#include "TreeVisitor.h"
#include "../Utils.h"

bool TreeVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    if (!astContext->getSourceManager().isInMainFile(func->getLocation())) {
        /* include functions only from the original header */
        return true;
    }
    std::string funcName = func->getNameInfo().getName().getAsString();
    std::shared_ptr<Type> retType =
        typeTranslator.translate(func->getReturnType());
    std::vector<std::shared_ptr<Parameter>> parameters;

    int anonCounter = 0;

    for (const clang::ParmVarDecl *parm : func->parameters()) {
        // Handle default values
        std::string pname = parm->getNameAsString();

        if (pname.empty()) {
            pname = "anonymous" + std::to_string(anonCounter++);
        }

        std::shared_ptr<Type> ptype = typeTranslator.translate(parm->getType());
        parameters.emplace_back(std::make_shared<Parameter>(pname, ptype));
    }

    ir.addFunction(funcName, std::move(parameters), retType,
                   func->isVariadic());

    return true;
}

bool TreeVisitor::VisitTypedefDecl(clang::TypedefDecl *tpdef) {
    std::string name = tpdef->getName();

    std::shared_ptr<Type> type =
        typeTranslator.translate(tpdef->getUnderlyingType());
    if (type) {
        ir.addTypeDef(name, type, typeTranslator.getLocation(tpdef));
    }
    return true;
}

bool TreeVisitor::VisitEnumDecl(clang::EnumDecl *enumdecl) {
    std::string name = enumdecl->getNameAsString();

    if (name.empty() && enumdecl->getTypedefNameForAnonDecl()) {
        name = enumdecl->getTypedefNameForAnonDecl()->getNameAsString();
    }

    std::vector<Enumerator> enumerators;

    for (const clang::EnumConstantDecl *en : enumdecl->enumerators()) {
        int64_t value = en->getInitVal().getSExtValue();
        enumerators.emplace_back(en->getNameAsString(), value);
    }

    std::string scalaType = typeTranslator.getTypeFromTypeMap(
        enumdecl->getIntegerType().getUnqualifiedType().getAsString());

    ir.addEnum(name, scalaType, std::move(enumerators),
               typeTranslator.getLocation(enumdecl));

    return true;
}

bool TreeVisitor::VisitRecordDecl(clang::RecordDecl *record) {
    std::string name = record->getNameAsString();

    // Handle typedef struct {} x; and typedef union {} y; by getting the name
    // from the typedef
    if ((record->isStruct() || record->isUnion()) && name.empty() &&
        record->getTypedefNameForAnonDecl()) {
        name = record->getTypedefNameForAnonDecl()->getNameAsString();
    }

    if (record->isUnion() && record->isThisDeclarationADefinition() &&
        !record->isAnonymousStructOrUnion() && !name.empty()) {
        typeTranslator.addUnionDefinition(record, name);
        return true;

    } else if (record->isStruct() && record->isThisDeclarationADefinition() &&
               !record->isAnonymousStructOrUnion() && !name.empty()) {
        typeTranslator.addStructDefinition(record, name);
        return true;
    }
    return false;
}

bool TreeVisitor::VisitVarDecl(clang::VarDecl *varDecl) {
    if (!astContext->getSourceManager().isInMainFile(varDecl->getLocation())) {
        /* include variables only from the original header */
        return true;
    }
    if (!varDecl->isThisDeclarationADefinition()) {
        std::string variableName = varDecl->getName().str();
        std::shared_ptr<Type> type =
            typeTranslator.translate(varDecl->getType());
        std::shared_ptr<Variable> variable = ir.addVariable(variableName, type);
        /* check if there is a macro for the variable.
         * Macros were saved by DefineFinder */
        std::string macroName = ir.getDefineForVar(variableName);
        if (!macroName.empty()) {
            ir.addVarDefine(macroName, variable);
        }
    }
    return true;
}
