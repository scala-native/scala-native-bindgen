#ifndef SCALA_NATIVE_BINDGEN_UNION_H
#define SCALA_NATIVE_BINDGEN_UNION_H

#include "Record.h"
#include "TypeAndName.h"
#include "TypeDef.h"
#include "types/ArrayType.h"
#include "types/PointerType.h"
#include <string>
#include <vector>

class Union : public Record, public ArrayType {
  public:
    Union(std::string name, std::vector<std::shared_ptr<Field>> fields,
          uint64_t maxSize, std::shared_ptr<Location> location);

    std::shared_ptr<TypeDef> generateTypeDef() override;

    std::string generateHelperClass() const override;

    bool operator==(const Type &other) const override;

    std::string getTypeAlias() const override;

    bool usesType(
        const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
        std::vector<std::shared_ptr<const Type>> &visitedTypes) const override;

  private:
    std::string generateGetter(const std::shared_ptr<Field> &field) const;

    std::string generateSetter(const std::shared_ptr<Field> &field) const;
};

#endif // SCALA_NATIVE_BINDGEN_UNION_H
