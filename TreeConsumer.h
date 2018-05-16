#pragma once

#include "TreeVisitor.h"
#include "Utils.h"

#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"

class TreeConsumer : public clang::ASTConsumer {
private:
    TreeVisitor *visitor;
    clang::SourceManager& smanager;

public:

    explicit TreeConsumer(clang::CompilerInstance *CI) : visitor(new TreeVisitor(CI)), smanager(CI->getASTContext().getSourceManager()) {}

    virtual bool HandleTopLevelDecl(clang::DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (clang::DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            clang::Decl *D = *i;
            std::string fpath = smanager.getFilename(D->getLocation()).str();
            locations.insert(basename(fpath) + "\n");

            if(!headerMan.IsStandard(basename(fpath)) && fpath != ""){
                visitor->TraverseDecl(D);
            }

        }
        return true;
    }


};
