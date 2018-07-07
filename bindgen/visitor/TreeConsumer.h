#pragma once

#include "../Utils.h"
#include "../ir/IR.h"
#include "TreeVisitor.h"

class TreeConsumer : public clang::ASTConsumer {
  private:
    TreeVisitor visitor;

    clang::SourceManager &smanager;

  public:
    TreeConsumer(clang::CompilerInstance *CI, IR &ir,
                 LocationManager &locationManager)
        : visitor(CI, ir, locationManager),
          smanager(CI->getASTContext().getSourceManager()) {}

    bool HandleTopLevelDecl(clang::DeclGroupRef DG) override {
        // a DeclGroupRef may have multiple Decls, so we iterate through each
        // one
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
