#ifndef SCALA_NATIVE_BINDGEN_STRUCT_H
#define SCALA_NATIVE_BINDGEN_STRUCT_H

#include "TypeAndName.h"
#include "TypeDef.h"
#include "types/ArrayType.h"
#include <string>
#include <vector>

#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22

class Field : public TypeAndName {
  public:
    Field(std::string name, Type *type);
};

class StructOrUnion {
  public:
    StructOrUnion(std::string name, std::vector<Field> fields);

    virtual TypeDef *generateTypeDef() = 0;

    virtual std::string generateHelperClass() const = 0;

    std::string getName() const;

  protected:
    std::string name;
    std::vector<Field> fields;
};

class Struct : public StructOrUnion, public Type {
  public:
    Struct(std::string name, std::vector<Field> fields, uint64_t typeSize);

    TypeDef *generateTypeDef() override;

    std::string generateHelperClass() const override;

    std::string getAliasType() const;

    /**
     * @return true if helper methods will be generated for this struct
     */
    bool hasHelperMethods() const;

    bool usesType(Type *type) const override;

  protected:
    std::string _str() const override;

  private:
    /* type size is needed if number of fields is bigger than 22 */
    uint64_t typeSize;
};

class Union : public StructOrUnion, public ArrayType {
  public:
    Union(std::string name, std::vector<Field> fields, uint64_t maxSize);

    TypeDef *generateTypeDef() override;

    std::string generateHelperClass() const override;

  private:
    std::string getTypeAlias() const;
};

#endif // SCALA_NATIVE_BINDGEN_STRUCT_H
