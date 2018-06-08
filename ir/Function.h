#ifndef SCALA_NATIVE_BINDGEN_FUNCTION_H
#define SCALA_NATIVE_BINDGEN_FUNCTION_H


#include <string>
#include <vector>
#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>

class Parameter : public TypeAndName {
public:
    Parameter(std::string name, std::string type);
};

class Function {
public:
    Function(std::string name, std::vector<Parameter> parameters,
             std::string retType, bool isVariadic);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const Function &func);

    bool usesType(const std::string &type) const;

    std::string getName() const;

private:

    std::string getVarargsParameterName() const;

    bool existParameterWithName(const std::string &parameterName) const;

    std::string name;
    std::vector<Parameter> parameters;
    std::string retType;
    bool isVariadic;
};


#endif //SCALA_NATIVE_BINDGEN_FUNCTION_H
