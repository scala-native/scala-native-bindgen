#pragma once

#include "../Utils.h"
#include "../ir/IR.h"
#include "TreeVisitor.h"

class TreeConsumer : public clang::ASTConsumer {
  private:
    TreeVisitor visitor;

    clang::SourceManager &smanager;

  public:
    TreeConsumer(clang::CompilerInstance *CI, IR &ir)
        : visitor(CI, ir), smanager(CI->getASTContext().getSourceManager()) {}

    bool HandleTopLevelDecl(clang::DeclGroupRef DG) override {
        // a DeclGroupRef may have multiple Decls, so we iterate through each
        // one
        for (auto D : DG) {
            visitor.TraverseDecl(D);
        }
        return true;
    }
};
