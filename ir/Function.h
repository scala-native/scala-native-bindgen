#ifndef SCALA_NATIVE_BINDGEN_FUNCTION_H
#define SCALA_NATIVE_BINDGEN_FUNCTION_H


#include <string>
#include <vector>
#include "TypeAndName.h"

class Parameter : public TypeAndName {
public:
    Parameter(std::string name, std::string type);
};

class Function {
public:
    Function(std::string name, std::vector<Parameter> parameters,
             std::string retType, bool isVariadic);

    friend std::ostream &operator<<(std::ostream &s, const Function &func);

private:
    std::string name;
    std::vector<Parameter> parameters;
    std::string retType;
    bool isVariadic;
};


#endif //SCALA_NATIVE_BINDGEN_FUNCTION_H
