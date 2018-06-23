#ifndef SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
#define SCALA_NATIVE_BINDGEN_LITERALDEFINE_H

#include "Define.h"
#include <llvm/Support/raw_ostream.h>

class LiteralDefine : public Define {
  public:
    LiteralDefine(std::string name, std::string literal, std::string type);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const LiteralDefine &literalDefine);

  private:
    std::string literal;
    std::string type;
};

#endif // SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
