#ifndef SCALA_NATIVE_BINDGEN_VARIABLE_H
#define SCALA_NATIVE_BINDGEN_VARIABLE_H

#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>

class Variable : public TypeAndName {
  public:
    Variable(const std::string &name, std::shared_ptr<const Type> type);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const Variable &variable);

    bool hasIllegalUsageOfOpaqueType() const;
};

#endif // SCALA_NATIVE_BINDGEN_VARIABLE_H
