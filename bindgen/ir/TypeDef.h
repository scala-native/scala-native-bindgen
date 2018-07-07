#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "TypeAndName.h"
#include "location/Location.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

class TypeDef : public TypeAndName, public Type {
  public:
    TypeDef(std::string name, std::shared_ptr<Type> type,
            std::shared_ptr<Location> location);

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s,
                                         const TypeDef &type);

    bool usesType(const std::shared_ptr<Type> &type,
                  bool stopOnTypeDefs) const override;

    std::string str() const override;

    bool operator==(const Type &other) const override;

    void setLocation(std::shared_ptr<Location> location);

    std::shared_ptr<Location> getLocation();

  private:
    /**
     * nullptr if type is located in main file or is generated.
     */
    std::shared_ptr<Location> location;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
