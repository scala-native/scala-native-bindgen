#include "LiteralDefine.h"

LiteralDefine::LiteralDefine(std::string name, std::string literal)
    : Define(std::move(name)), literal(std::move(literal)) {}

LiteralDefine::LiteralDefine(std::string name, std::string literal,
                             std::string type)
    : Define(std::move(name)), literal(std::move(literal)),
      type(std::move(type)) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                              const LiteralDefine &literalDefine) {
    s << "  val " << literalDefine.name;
    if (!literalDefine.type.empty()) {
        s << ": " << literalDefine.type;
    }
    s << " = " << literalDefine.literal << "\n";
    return s;
}
