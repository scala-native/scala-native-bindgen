#pragma once

#include "TypeTranslator.h"

#include <map>
#include <set>
#include <string>

class CycleDetection {
  private:
    TypeTranslator &tpeTransl;

    bool contains(const std::string &k) const {
        return dependencies.count(k) != 0;
    }

  public:
    std::map<std::string, std::set<std::string>> dependencies;

    explicit CycleDetection(TypeTranslator &tpeTransl_)
        : tpeTransl(tpeTransl_), dependencies{} {}

    void AddDependency(const std::string &name, const clang::QualType &qtpe) {

        if (qtpe->isFunctionPointerType()) {
            // TODO: function pointer
            /* type translator cannot translate function type */
            return;
        }

        if (qtpe->isPointerType()) {
            const auto *ptr = qtpe.getTypePtr()->getAs<clang::PointerType>();
            AddDependency(name, ptr->getPointeeType());
            return;
        }

        Type *type = tpeTransl.translate(qtpe);
        std::string qtpeString = type->str();

        // Add the dependence of qtpe
        if (contains(qtpeString)) {
            dependencies[name].insert(dependencies[qtpeString].begin(),
                                      dependencies[qtpeString].end());
        }

        dependencies[name].insert(qtpeString);
    }

    bool isCyclic(const std::string &name) {
        if (contains(name)) {
            if (dependencies[name].count(name) != 0) {
                return true;
            }
        }
        return false;
    }
};
