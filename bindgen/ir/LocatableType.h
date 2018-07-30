#ifndef SCALA_NATIVE_BINDGEN_LOCATABLE_H
#define SCALA_NATIVE_BINDGEN_LOCATABLE_H

#include "Location.h"
#include "types/Type.h"

class LocatableType : virtual public Type {
  public:
    explicit LocatableType(std::shared_ptr<Location> location);

    virtual std::shared_ptr<Location> getLocation() const;

  protected:
    /**
     * nullptr if type is generated
     */
    std::shared_ptr<Location> location;
};

#endif // SCALA_NATIVE_BINDGEN_LOCATABLE_H
