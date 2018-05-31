#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include <string>
#include <vector>
#include "TypeAndName.h"
#include "TypeDef.h"

#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22

class Field : public TypeAndName {
public:
    Field(std::string name, std::string type);
};

class StructOrUnion {
public:
    StructOrUnion(std::string name, std::vector<Field> fields);

    virtual TypeDef generateTypeDef() = 0;

    virtual std::string generateHelperClass() const = 0;

protected:
    std::string name; // names of structs and unions are not empty
    std::vector<Field> fields;
};

class Struct : StructOrUnion {
public:
    Struct(std::string name, std::vector<Field> fields, uint64_t typeSize);

    TypeDef generateTypeDef() override;

    std::string generateHelperClass() const override;

private:
    std::string getFieldsTypes();

    /* type size is needed if number of fields is bigger than 22 */
    uint64_t typeSize;
};

class Union : StructOrUnion {
public:
    Union(std::string name, std::vector<Field> members, uint64_t maxSize);

    TypeDef generateTypeDef() override;

    std::string generateHelperClass() const override;

private:
    uint64_t maxSize;
};


#endif //SCALA_NATIVE_BINDGEN_STRUCT_H
