#pragma once

#include "../CycleDetection.h"
#include "../TypeTranslator.h"
#include "../ir/IR.h"
#include "../ir/location/LocationManager.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>

class TreeVisitor : public clang::RecursiveASTVisitor<TreeVisitor> {
  private:
    clang::ASTContext *astContext;
    TypeTranslator typeTranslator;
    CycleDetection cycleDetection;
    IR &ir;
    LocationManager &locationManager;

    void handleUnion(clang::RecordDecl *record, std::string name);

    void handleStruct(clang::RecordDecl *record, std::string name);

    std::shared_ptr<Location> getLocation(clang::Decl *decl);

  public:
    TreeVisitor(clang::CompilerInstance *CI, IR &ir,
                LocationManager &locationManager)
        : astContext(&(CI->getASTContext())), typeTranslator(astContext, ir),
          cycleDetection(typeTranslator), ir(ir),
          locationManager(locationManager) {}

    virtual ~TreeVisitor() = default;

    virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
    virtual bool VisitTypedefDecl(clang::TypedefDecl *tpdef);
    virtual bool VisitEnumDecl(clang::EnumDecl *enumdecl);
    virtual bool VisitRecordDecl(clang::RecordDecl *record);
    virtual bool VisitVarDecl(clang::VarDecl *VD);
};
