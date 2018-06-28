#ifndef SCALA_NATIVE_BINDGEN_FUNCTION_H
#define SCALA_NATIVE_BINDGEN_FUNCTION_H

#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>
#include <vector>

class Parameter : public TypeAndName {
  public:
    Parameter(std::string name, Type *type);
};

class Function {
  public:
    Function(const std::string &name, std::vector<Parameter *> parameters,
             Type *retType, bool isVariadic);

    ~Function();

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const Function &func);

    bool usesType(Type *type) const;

    std::string getName() const;

    void setScalaName(std::string scalaName);

    void deallocateTypesThatAreNotInIR();

  private:
    std::string getVarargsParameterName() const;

    bool existsParameterWithName(const std::string &parameterName) const;

    std::string name;      // real name of the function
    std::string scalaName; // not empty
    std::vector<Parameter *> parameters;
    Type *retType;
    bool isVariadic;
};

#endif // SCALA_NATIVE_BINDGEN_FUNCTION_H
