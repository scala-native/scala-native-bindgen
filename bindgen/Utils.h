#ifndef UTILS_H
#define UTILS_H

#include "ir/TypeDef.h"
#include "ir/types/Type.h"
#include <clang/AST/AST.h>

inline std::string uint64ToScalaNat(uint64_t v, std::string accumulator = "") {
    if (v == 0)
        return accumulator;

    auto last_digit = v % 10;
    auto rest = v / 10;

    if (accumulator.empty()) {
        return uint64ToScalaNat(rest,
                                "native.Nat._" + std::to_string(last_digit));
    } else {
        return uint64ToScalaNat(rest, "native.Nat.Digit[native.Nat._" +
                                          std::to_string(last_digit) + ", " +
                                          accumulator + "]");
    }
}

inline bool typeEquals(const clang::Type *tpe1, const std::string *tpe2) {
    if (tpe1 == nullptr && tpe2 == nullptr) {
        return true;
    }
    if (tpe1 == nullptr || tpe2 == nullptr) {
        return false;
    }
    // TODO: What is the proper way ?
    if (tpe1->getAsTagDecl()) {
        return tpe1->getAsTagDecl()->getNameAsString() == *tpe2;
    }
    return false;
}

static std::array<std::string, 39> reserved_words = {
    {"abstract",  "case",    "catch",    "class",    "def",     "do",
     "else",      "extends", "false",    "final",    "finally", "for",
     "forSome",   "if",      "implicit", "import",   "lazy",    "match",
     "new",       "null",    "object",   "override", "package", "private",
     "protected", "return",  "sealed",   "super",    "this",    "throw",
     "trait",     "try",     "true",     "type",     "val",     "var",
     "while",     "with",    "yield"}};

inline std::string handleReservedWords(const std::string &name,
                                       const std::string &suffix = "") {
    auto found = std::find(reserved_words.begin(), reserved_words.end(), name);
    if (found != reserved_words.end()) {
        return "`" + name + suffix + "`";
    } else {
        return name + suffix;
    }
}

/**
 * @return true if str starts with given prefix
 */
static inline bool startsWith(const std::string &str,
                              const std::string &prefix) {
    return str.substr(0, prefix.size()) == prefix;
}

template <typename T, typename PT> static inline bool isInstanceOf(PT *type) {
    auto *p = dynamic_cast<const T *>(type);
    return p != nullptr;
}

static inline std::string replaceChar(const std::string &str,
                                      const std::string &c1,
                                      const std::string &c2) {
    auto f = str.find(c1);
    if (f != std::string::npos) {
        return std::string(str).replace(f, c1.length(), c2);
    }
    return std::string(str);
}

/**
 * Types may be wrapper in a chain of typedefs.
 * @return true if given type is of type T or is an alias for type T.
 */
template <typename T> static inline bool isAliasForType(const Type *type) {
    if (isInstanceOf<T>(type)) {
        return true;
    }
    auto *typeDef = dynamic_cast<const TypeDef *>(type);
    if (typeDef) {
        return isAliasForType<const T>(typeDef->getType().get());
    }
    return false;
}

/**
 * @return true if typedef references opaque type directly or through a
 * chain of typedefs.
 */
static inline bool isAliasForOpaqueType(const Type *type) {
    assert(type);
    auto *typeDef = dynamic_cast<const TypeDef *>(type);
    if (typeDef) {
        if (!typeDef->getType()) {
            return true;
        }
        return isAliasForOpaqueType(typeDef->getType().get());
    }
    return false;
}

#endif // UTILS_H
