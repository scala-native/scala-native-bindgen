#ifndef UTILS_H
#define UTILS_H

#include "clang/AST/AST.h"

#include <string>
#include <cinttypes>

inline std::string basename(const std::string& pathname) {
    return {std::find_if(pathname.rbegin(), pathname.rend(),
                         [](char c) { return c == '/'; }).base(),
            pathname.end()};
}

inline std::string intToScalaNat(int v, std::string accumulator = ""){

    if(v > 0){
        int last_digit = v % 10;
        int rest = v / 10;

        if(accumulator == ""){
            return intToScalaNat(rest, "_" + std::to_string(last_digit));
        } else{
            return intToScalaNat(rest, "Digit[_" + std::to_string(last_digit) + ", " + accumulator + "]");
        }
    } else{
        return accumulator;
    }
}

inline std::string uint64ToScalaNat(uint64_t v, std::string accumulator = ""){

    if(v > 0){
        int last_digit = v % 10;
        int rest = v / 10;

        if(accumulator == ""){
            return uint64ToScalaNat(rest, "_" + std::to_string(last_digit));
        } else{
            return uint64ToScalaNat(rest, "Digit[_" + std::to_string(last_digit) + ", " + accumulator + "]");
        }
    } else{
        return accumulator;
    }
}


inline bool typeEquals(const clang::Type* tpe1, const std::string* tpe2){
    if(tpe1 == nullptr && tpe2 == nullptr){
        return true;
    }
    if(tpe1 == nullptr || tpe2 == nullptr){
        return false;
    }
    //TODO: What is the proper way ?
    if(tpe1->getAsTagDecl() && tpe2){
        return tpe1->getAsTagDecl()->getNameAsString() == *tpe2;
    }
    return false;
}

#endif // UTILS_H
