#ifndef SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
#define SCALA_NATIVE_BINDGEN_LITERALDEFINE_H

#include "Define.h"
#include "types/Type.h"
#include <llvm/Support/raw_ostream.h>

class LiteralDefine : public Define {
  public:
    LiteralDefine(std::string name, std::string literal,
                  std::shared_ptr<Type> type);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const LiteralDefine &literalDefine);

  private:
    std::string literal;
    std::shared_ptr<Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
