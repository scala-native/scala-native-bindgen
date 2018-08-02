#pragma once

#include "../TypeTranslator.h"
#include "../ir/IR.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>

class TreeVisitor : public clang::RecursiveASTVisitor<TreeVisitor> {
  private:
    clang::ASTContext *astContext;
    TypeTranslator typeTranslator;
    IR &ir;

  public:
    TreeVisitor(clang::CompilerInstance *CI, IR &ir)
        : astContext(&(CI->getASTContext())), typeTranslator(astContext, ir),
          ir(ir) {}

    virtual ~TreeVisitor() = default;

    virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
    virtual bool VisitTypedefDecl(clang::TypedefDecl *tpdef);
    virtual bool VisitEnumDecl(clang::EnumDecl *enumdecl);
    virtual bool VisitRecordDecl(clang::RecordDecl *record);
    virtual bool VisitVarDecl(clang::VarDecl *VD);
};
