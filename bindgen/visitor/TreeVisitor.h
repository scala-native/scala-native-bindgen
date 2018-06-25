#pragma once

#include "../CycleDetection.h"
#include "../HeaderManager.h"
#include "../TypeTranslator.h"
#include "../ir/IR.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>

extern HeaderManager headerMan;

extern std::set<std::string> locations;

class TreeVisitor : public clang::RecursiveASTVisitor<TreeVisitor> {
  private:
    clang::ASTContext *astContext;
    TypeTranslator typeTranslator;
    CycleDetection cycleDetection;
    IR &ir;

    void handleUnion(clang::RecordDecl *record, std::string name);

    void handleStruct(clang::RecordDecl *record, std::string name);

  public:
    TreeVisitor(clang::CompilerInstance *CI, IR &ir)
        : astContext(&(CI->getASTContext())), typeTranslator(astContext, ir),
          cycleDetection(typeTranslator), ir(ir) {}

    virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
    virtual bool VisitTypedefDecl(clang::TypedefDecl *tpdef);
    virtual bool VisitEnumDecl(clang::EnumDecl *enumdecl);
    virtual bool VisitRecordDecl(clang::RecordDecl *record);
    virtual bool VisitVarDecl(clang::VarDecl *VD);
};
