#pragma once

#include "TypeTranslator.h"

#include <map>
#include <set>
#include <string>

class CycleDetection {
  private:
    TypeTranslator &tpeTransl;

    bool contains(std::string &k) { return !!dependencies.count(k); }

  public:
    std::map<std::string, std::set<std::string>> dependencies;
    CycleDetection(TypeTranslator &tpeTransl_)
        : tpeTransl(tpeTransl_), dependencies{} {}

    void AddDependcy(std::string name, const clang::QualType &qtpe) {

        // TODO: function pointer

        if (qtpe->isPointerType()) {
            const clang::PointerType *ptr =
                qtpe.getTypePtr()->getAs<clang::PointerType>();
            AddDependcy(name, ptr->getPointeeType());
            return;
        }

        std::string qtpeString = tpeTransl.Translate(qtpe);

        // Add the dependence of qtpe
        if (contains(qtpeString)) {
            dependencies[name].insert(dependencies[qtpeString].begin(),
                                      dependencies[qtpeString].end());
        }

        dependencies[name].insert(qtpeString);
    }

    bool isCyclic(std::string &name) {
        if (contains(name)) {
            if (dependencies[name].count(name) != 0) {
                return true;
            }
        }
        return false;
    }
};
