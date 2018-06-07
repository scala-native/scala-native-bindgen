#pragma once

#include "TreeVisitor.h"
#include "../Utils.h"

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include "../ir/IR.h"

class TreeConsumer : public clang::ASTConsumer {
private:
    TreeVisitor visitor;

    clang::SourceManager &smanager;

public:

    TreeConsumer(clang::CompilerInstance *CI, IR *ir)
            : visitor(CI, ir),
              smanager(CI->getASTContext().getSourceManager()) {}

    bool HandleTopLevelDecl(clang::DeclGroupRef DG) override {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (auto D : DG) {
            std::string fpath = smanager.getFilename(D->getLocation()).str();

            if (!headerMan.IsStandard(basename(fpath)) && !fpath.empty()) {
                locations.insert(basename(fpath) + "\n");
                visitor.TraverseDecl(D);
            }

        }
        return true;
    }


};
