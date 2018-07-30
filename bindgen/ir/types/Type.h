#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <memory>
#include <string>

/**
 * Base class for types.
 */
class Type : public std::enable_shared_from_this<Type> {
  public:
    virtual std::string str() const = 0;

    /**
     * @param stopOnTypeDefs if this parameter is true then TypeDefs instances
     *                       will not be checked. This parameter is needed when
     *                       usages of TypeDefs are checked, it helps to avoid
     *                       false positives when usages if aliases for the
     *                       typedef are found.
     */
    virtual bool usesType(const std::shared_ptr<const Type> &type,
                          bool stopOnTypeDefs) const = 0;

    virtual bool operator==(const Type &other) const = 0;

    virtual bool operator!=(const Type &other) const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
