#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "LocatableType.h"
#include "Location.h"
#include "TypeAndName.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName, public LocatableType {
  public:
    /**
     * @param location nullptr if typedef is generated
     */
    TypeDef(std::string name, std::shared_ptr<const Type> type,
            std::shared_ptr<Location> location);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const TypeDef &type);

    bool usesType(const std::shared_ptr<const Type> &type,
                  bool stopOnTypeDefs) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

    /**
     * @return true if the typedef is not generated alias for opaque type.
     */
    bool hasLocation() const;

    /**
     * @return location of the typedef is it is not generated otherwise
     *         location of referenced type (struct, union or enum).
     */
    std::shared_ptr<Location> getLocation() const override;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
