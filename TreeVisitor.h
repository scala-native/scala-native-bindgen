#pragma once

#include "HeaderManager.h"
#include "TypeTranslator.h"

#include "clang/Driver/Options.h"
#include "clang/Basic/LangOptions.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"

extern HeaderManager headerMan;

extern std::string declarations;
extern std::string enums;
extern std::string helpers;

class TreeVisitor : public clang::RecursiveASTVisitor<TreeVisitor> {
private:
    clang::ASTContext* astContext;
    TypeTranslator typeTranslator;

public:
    explicit TreeVisitor(clang::CompilerInstance *CI) : astContext(&(CI->getASTContext())), typeTranslator(astContext) {}

    virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
    virtual bool VisitTypedefDecl(clang::TypedefDecl *tpdef);
    virtual bool VisitEnumDecl(clang::EnumDecl *enumdecl);
    virtual bool VisitRecordDecl(clang::RecordDecl *record);
};
