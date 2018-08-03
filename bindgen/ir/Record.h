#ifndef SCALA_NATIVE_BINDGEN_RECORD_H
#define SCALA_NATIVE_BINDGEN_RECORD_H

#include "LocatableType.h"
#include "TypeAndName.h"
#include "TypeDef.h"
#include <string>
#include <vector>

class Field : public TypeAndName {
  public:
    Field(std::string name, std::shared_ptr<const Type> type);

    Field(std::string name, std::shared_ptr<const Type> type,
          uint64_t offsetInBits);

    uint64_t getOffsetInBits() const;

  protected:
    /**
     * Offset in bytes from address of struct/union.
     */
    uint64_t offsetInBits = 0;
};

class Record : public LocatableType {
  public:
    Record(std::string name, std::vector<std::shared_ptr<Field>> fields,
           std::shared_ptr<Location> location);

    virtual std::shared_ptr<TypeDef> generateTypeDef() = 0;

    virtual std::string generateHelperClass() const = 0;

    std::string getName() const;

    virtual std::string getTypeAlias() const = 0;

    virtual bool hasHelperMethods() const;

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  protected:
    std::string name;
    std::vector<std::shared_ptr<Field>> fields;
};

#endif // SCALA_NATIVE_BINDGEN_RECORD_H
