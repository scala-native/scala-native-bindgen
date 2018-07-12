#ifndef SCALA_NATIVE_BINDGEN_TYPEDEF_H
#define SCALA_NATIVE_BINDGEN_TYPEDEF_H

#include "Location.h"
#include "TypeAndName.h"
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

    std::shared_ptr<Location> getLocation() const;

    /**
     * @tparam T Type
     * @return true if the typedef is an alias for give type directly or through
     * a chain of typedefs
     */
    template <typename T> bool isAliasFor() const {
        /* if body is moved to cpp file then linker gives undefined symbols
         * error */
        if (!type) {
            return false;
        }
        if (dynamic_cast<T *>(type.get())) {
            return true;
        }
        auto *typeDef = dynamic_cast<TypeDef *>(type.get());
        if (typeDef) {
            return typeDef->isAliasFor<T>();
        }
        return false;
    }

  private:
    /**
     * nullptr if type is generated.
     */
    std::shared_ptr<Location> location;
};

#endif // SCALA_NATIVE_BINDGEN_TYPEDEF_H
