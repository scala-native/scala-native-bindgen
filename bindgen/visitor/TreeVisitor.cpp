#include "TreeVisitor.h"

HeaderManager headerMan;

std::set<std::string> locations;

bool TreeVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    std::string funcName = func->getNameInfo().getName().getAsString();
    Type *retType = typeTranslator.translate(func->getReturnType());
    std::vector<Parameter> parameters;

    int anonCounter = 0;

    for (const clang::ParmVarDecl *parm : func->parameters()) {
        // Handle default values
        std::string pname = parm->getNameAsString();

        if (pname.empty()) {
            pname = "anonymous" + std::to_string(anonCounter++);
        }

        Type *ptype = typeTranslator.translate(parm->getType());
        parameters.emplace_back(pname, ptype);
    }

    ir.addFunction(funcName, std::move(parameters), retType,
                   func->isVariadic());

    return true;
}

bool TreeVisitor::VisitTypedefDecl(clang::TypedefDecl *tpdef) {
    std::string name = tpdef->getName();

    cycleDetection.AddDependcy(name, tpdef->getUnderlyingType());
    if (cycleDetection.isCyclic(name)) {
        llvm::errs() << "Error: " << name << " is cyclic\n";
        llvm::errs() << name << "\n";
        for (auto &s : cycleDetection.dependencies[name]) {
            llvm::errs() << "\t" << s << "\n";
        }
        llvm::errs() << cycleDetection.isCyclic(name) << "\n";
        llvm::errs().flush();
    }

    Type *type = typeTranslator.translate(tpdef->getUnderlyingType());
    ir.addTypeDef(name, type);
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

    Type *alias = ir.addEnum(name, scalaType, std::move(enumerators));
    if (alias != nullptr) {
        typeTranslator.addAlias("enum " + name, alias);
    }

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

    if (record->isUnion() && !record->isAnonymousStructOrUnion() &&
        !name.empty()) {
        handleUnion(record, name);
        return true;

    } else if (record->isStruct() && record->isThisDeclarationADefinition() &&
               !record->isAnonymousStructOrUnion() && !name.empty()) {
        handleStruct(record, name);
        return true;
    }
    return false;
}

void TreeVisitor::handleUnion(clang::RecordDecl *record, std::string name) {
    uint64_t maxSize = 0;

    std::vector<Field> fields;

    for (const clang::FieldDecl *field : record->fields()) {
        uint64_t sizeInBytes = astContext->getTypeSize(field->getType()) / 8;
        maxSize = std::max(maxSize, sizeInBytes);
        std::string fname = field->getNameAsString();
        Type *ftype = typeTranslator.translate(field->getType(), &name);

        fields.emplace_back(fname, ftype);
    }

    Type *alias = ir.addUnion(name, std::move(fields), maxSize);

    typeTranslator.addAlias("union " + name, alias);
}

void TreeVisitor::handleStruct(clang::RecordDecl *record, std::string name) {
    std::string newName = "struct_" + name;

    if (record->hasAttr<clang::PackedAttr>()) {
        llvm::errs() << "Warning: struct " << name << " is packed. "
                     << "Packed structs are not supported by Scala Native. "
                     << "Access to fields will not work correctly.\n";
        llvm::errs().flush();
    }

    int fieldCnt = 0;
    std::vector<Field> fields;

    for (const clang::FieldDecl *field : record->fields()) {
        Type *ftype = typeTranslator.translate(field->getType(), &name);
        fields.emplace_back(field->getNameAsString(), ftype);

        cycleDetection.AddDependcy(newName, field->getType());

        fieldCnt++;
    }

    if (cycleDetection.isCyclic(newName)) {
        llvm::errs() << "Error: " << newName << " is cyclic\n";
        llvm::errs() << newName << "\n";
        for (auto &s : cycleDetection.dependencies[newName]) {
            llvm::errs() << "\t" << s << "\n";
        }
        llvm::errs() << cycleDetection.isCyclic(newName) << "\n";
        llvm::errs().flush();
    }

    Type *alias =
        ir.addStruct(name, std::move(fields),
                     astContext->getTypeSize(record->getTypeForDecl()));

    typeTranslator.addAlias("struct " + name, alias);
}

bool TreeVisitor::VisitVarDecl(clang::VarDecl *varDecl) {
    if (!varDecl->isThisDeclarationADefinition()) {
        std::string variableName = varDecl->getName().str();
        Type *type = typeTranslator.translate(varDecl->getType());
        Variable *variable = ir.addVariable(variableName, type);
        /* check if there is a macro for the variable.
         * Macros were saved by DefineFinder */
        std::string macroName = ir.getDefineForVar(variableName);
        if (!macroName.empty()) {
            ir.addVarDefine(macroName, variable);
        }
    }
    return true;
}
