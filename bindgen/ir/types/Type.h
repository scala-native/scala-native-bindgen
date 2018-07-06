#ifndef SCALA_NATIVE_BINDGEN_TYPE_H
#define SCALA_NATIVE_BINDGEN_TYPE_H

#include <memory>
#include <string>

/**
 * Base class for types.
 */
class Type {
  public:
    virtual ~Type() = default;

    virtual std::string str() const;

    /**
     * @param stopOnTypeDefs if this parameter is true then TypeDefs instances
     *                       will not be checked. This parameter is needed when
     *                       usages of TypeDefs are checked, it helps to avoid
     *                       false positives when usages if aliases for the
     *                       typedef are found.
     */
    virtual bool usesType(const std::shared_ptr<Type> &type,
                          bool stopOnTypeDefs) const;

    virtual bool operator==(const Type &other) const;

    virtual bool operator!=(const Type &other) const;
};

#endif // SCALA_NATIVE_BINDGEN_TYPE_H
