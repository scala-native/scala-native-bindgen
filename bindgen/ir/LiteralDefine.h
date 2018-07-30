#ifndef SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
#define SCALA_NATIVE_BINDGEN_LITERALDEFINE_H

#include "Define.h"
#include "types/Type.h"
#include <llvm/Support/raw_ostream.h>

class LiteralDefine : public Define {
  public:
    LiteralDefine(std::string name, std::string literal,
                  std::shared_ptr<const Type> type);

    std::string getDefinition(const LocationManager &locationManager) const;

    bool usesType(const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
                  std::vector<std::shared_ptr<const Type>> &visitedTypes) const;

  private:
    std::string literal;
    std::shared_ptr<const Type> type;
};

#endif // SCALA_NATIVE_BINDGEN_LITERALDEFINE_H
