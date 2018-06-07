#include "TreeVisitor.h"
#include "../Utils.h"

HeaderManager headerMan;

std::set<std::string> locations;

bool TreeVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    std::string funcName = func->getNameInfo().getName().getAsString();
    std::string retType = handleReservedWords(typeTranslator.Translate(func->getReturnType()));
    std::vector<Parameter> parameters;

    int anonCounter = 0;

    for (const clang::ParmVarDecl *parm : func->parameters()) {
        //Handle default values
        std::string pname = parm->getNameAsString();

        if (pname.empty()) {
            pname = "anonymous" + std::to_string(anonCounter++);
        }

        std::string ptype = handleReservedWords(typeTranslator.Translate(parm->getType()));
        parameters.push_back(Parameter(pname, ptype));
    }

    ir->addFunction(funcName, parameters, retType, func->isVariadic());

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

    std::string type = handleReservedWords(typeTranslator.Translate(tpdef->getUnderlyingType()));
    ir->addTypeDef(name, type);
    return true;
}

bool TreeVisitor::VisitEnumDecl(clang::EnumDecl *enumdecl) {
    std::string name = enumdecl->getNameAsString();

    if (name.empty() && enumdecl->getTypedefNameForAnonDecl()) {
        name = enumdecl->getTypedefNameForAnonDecl()->getNameAsString();
    }

    if (!name.empty()) {
        // Replace "enum x" with enum_x in scala
        typeTranslator.AddTranslation("enum " + name, "enum_" + name);
    }

    std::vector<Enumerator> enumerators;

    for (const clang::EnumConstantDecl *en : enumdecl->enumerators()) {
        int64_t value = en->getInitVal().getSExtValue();
        enumerators.emplace_back(en->getNameAsString(), value);
    }

    ir->addEnum(name, typeTranslator.Translate(enumdecl->getIntegerType()), enumerators);

    return true;
}

bool TreeVisitor::VisitRecordDecl(clang::RecordDecl *record) {
    std::string name = record->getNameAsString();

    //Handle typedef struct {} x; and typedef union {} y; by getting the name from the typedef
    if ((record->isStruct() || record->isUnion()) && name.empty() && record->getTypedefNameForAnonDecl()) {
        name = record->getTypedefNameForAnonDecl()->getNameAsString();
    }

    if (record->isUnion() && !record->isAnonymousStructOrUnion() && !name.empty()) {
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
    // Replace "union x" with union_x in scala
    typeTranslator.AddTranslation("union " + name, "union_" + name);

    uint64_t maxSize = 0;

    std::vector<Field> fields;

    for (const clang::FieldDecl *field : record->fields()) {
        maxSize = std::max(maxSize, astContext->getTypeSize(field->getType()));
        std::string fname = field->getNameAsString();
        std::string ftype = handleReservedWords(typeTranslator.Translate(field->getType(), &name));

        fields.push_back(Field(fname, ftype));
    }

    ir->addUnion(name, fields, maxSize);
}

void TreeVisitor::handleStruct(clang::RecordDecl *record, std::string name) {
    std::string newName = "struct_" + name;

    // Replace "struct x" with struct_x in scala
    typeTranslator.AddTranslation("struct " + name, newName);

    int fieldCnt = 0;
    std::vector<Field> fields;

    for (const clang::FieldDecl *field : record->fields()) {
        std::string ftype = handleReservedWords(typeTranslator.Translate(field->getType(), &name));
        fields.push_back(Field(field->getNameAsString(), ftype));

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

    //llvm::errs() << newName << "\n";
    //for(auto& s : cycleDetection.dependencies[newName]){
    //    llvm::errs() << "\t" << s << "\n";
    //}
    //llvm::errs() << cycleDetection.isCyclic(newName) << "\n";

    ir->addStruct(name, fields, astContext->getTypeSize(record->getTypeForDecl()));
}
