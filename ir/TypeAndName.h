#ifndef SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
#define SCALA_NATIVE_BINDGEN_TYPEANDNAME_H


#include <string>

/**
 * Base class for function parameters, struct fields
 * and type declarations
 */
class TypeAndName {
public:
    TypeAndName(std::string name, std::string type);

    std::string getType() const;

    void setType(std::string name);

    std::string getName() const;

protected:
    std::string name;
    std::string type;
};


#endif //SCALA_NATIVE_BINDGEN_TYPEANDNAME_H
