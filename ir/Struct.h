#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include "TypeAndName.h"
#include "TypeDef.h"
#include <string>
#include <vector>

#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22

class Field : public TypeAndName {
  public:
    Field(std::string name, std::string type);
};

class StructOrUnion {
  public:
    StructOrUnion(std::string name, std::vector<Field> fields);

    virtual TypeDef generateTypeDef() const = 0;

    virtual std::string generateHelperClass() const = 0;

    std::string getName() const;

    virtual std::string getType() const = 0;

    /**
     * @return true if at leas one field has given type
     */
    bool usesType(const std::string &type) const;

  protected:
    std::string name; // names of structs and unions are not empty
    std::vector<Field> fields;
};

class Struct : public StructOrUnion {
  public:
    Struct(std::string name, std::vector<Field> fields, uint64_t typeSize);

    TypeDef generateTypeDef() const override;

    std::string generateHelperClass() const override;

    std::string getType() const override;

    /**
     * @return true if helper methods will be generated for this struct
     */
    bool hasHelperMethods() const;

  private:
    std::string getFieldsTypes() const;

    /* type size is needed if number of fields is bigger than 22 */
    uint64_t typeSize;
};

class Union : public StructOrUnion {
  public:
    Union(std::string name, std::vector<Field> members, uint64_t maxSize);

    TypeDef generateTypeDef() const override;

    std::string generateHelperClass() const override;

    std::string getType() const override;

  private:
    uint64_t maxSize;
};

#endif // SCALA_NATIVE_BINDGEN_STRUCT_H
