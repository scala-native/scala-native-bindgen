#include "LiteralDefine.h"

LiteralDefine::LiteralDefine(std::string name, std::string literal, Type *type)
    : Define(std::move(name)), literal(std::move(literal)), type(type) {}

llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                              const LiteralDefine &literalDefine) {
    s << "  val " << literalDefine.name << ": " << literalDefine.type->str()
      << " = " << literalDefine.literal << "\n";
    return s;
}

void LiteralDefine::deallocateTypesThatAreNotInIR() {
    if (type->canBeDeallocated()) {
        delete type;
    }
}
