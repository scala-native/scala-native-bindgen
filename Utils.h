#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cinttypes>

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

#endif // UTILS_H
