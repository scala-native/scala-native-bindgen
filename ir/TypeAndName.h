#ifndef SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
#define SCALA_NATIVE_BINDGEN_TYPEANDNAME_H


#include <string>
#include <clang/AST/Type.h>

/**
 * Base class for function parameters, struct fields
 * and type declarations
 */
class TypeAndName {
public:
    TypeAndName(std::string name, std::string type);

    std::string getType() const;

    std::string getName() const;

protected:
    std::string name;
    std::string type;
};


#endif //SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
