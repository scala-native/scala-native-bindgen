#ifndef UTILS_H
#define UTILS_H

#include <clang/AST/AST.h>

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <locale>
#include <string>

inline std::string basename(const std::string &pathname) {
    return {std::find_if(pathname.rbegin(), pathname.rend(),
                         [](char c) { return c == '/'; })
                .base(),
            pathname.end()};
}

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
    if (tpe1->getAsTagDecl() && tpe2) {
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

inline std::string handleReservedWords(std::string name,
                                       std::string suffix = "") {
    auto found = std::find(reserved_words.begin(), reserved_words.end(), name);
    if (found != reserved_words.end()) {
        return "`" + name + suffix + "`";
    } else {
        return name + suffix;
    }
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

/**
 * @return true if str starts with given prefix
 */
static inline bool startsWith(const std::string &str,
                              const std::string &prefix) {
    return str.substr(0, prefix.size()) == prefix;
}

/**
 * @return true if checkedType uses type
 *         example: checkedType = native.Ptr[struct_A], type = struct_A
 */
static inline bool typeUsesOtherType(const std::string &checkedType,
                                     const std::string &type) {
    // TODO: find better way to check it
    return checkedType == type || checkedType == "native.Ptr[" + type + "]" ||
           startsWith(checkedType, "native.CArray[" + type + ", ");
}

#endif // UTILS_H
