#ifndef SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H
#define SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H

#include "Enum.h"
#include "Function.h"
#include "LiteralDefine.h"
#include "PossibleVarDefine.h"
#include "Struct.h"
#include "TypeDef.h"
#include "VarDefine.h"

/**
 * Intermediate representation
 */
class IR {
  public:
    explicit IR(std::string libName, std::string linkName,
                std::string objectName, std::string packageName);

    ~IR();

    void addFunction(std::string name, std::vector<Parameter> parameters,
                     Type *retType, bool isVariadic);

    void addTypeDef(std::string name, Type *type);

    /**
     * @return type alias for the enum
     */
    Type *addEnum(std::string name, const std::string &type,
                  std::vector<Enumerator> enumerators);

    /**
     * @return type alias for the struct
     */
    Type *addStruct(std::string name, std::vector<Field> fields,
                    uint64_t typeSize);

    /**
     * @return type alias for the union
     */
    Type *addUnion(std::string name, std::vector<Field> fields,
                   uint64_t maxSize);

    void addLiteralDefine(std::string name, std::string literal, Type *type);

    void addPossibleVarDefine(const std::string &macroName,
                              const std::string &varName);

    void addVarDefine(std::string name, Variable *variable);

    Variable *addVariable(const std::string &name, const std::string &type);

    /**
     * @return true if there are no functions, types,
     *         structs and unions
     */
    bool libObjEmpty() const;

    bool hasEnums() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir);

    void generate(const std::string &excludePrefix);

    void removeDefine(const std::string &name);

    /**
     * @return macro name if there is a macro for the variable.
     *         otherwise return empty string
     */
    std::string getDefineForVar(const std::string &varName) const;

    Enum *getEnumWithName(const std::string &name);

    Struct *getStructWithName(const std::string &name);

    Union *getUnionWithName(const std::string &name);

    TypeDef *getTypeDefWithName(const std::string &name);

  private:
    /**
     * Generates type defs for enums, structs and unions
     */
    void generateTypeDefs();

    /**
     * @return true if helper methods will be generated for this library
     */
    bool hasHelperMethods() const;

    /**
     * Remove functions that start with given prefix.
     *
     * Replace typedef by underlying type if it starts with given prefix
     * and it is used only in other typedefs.
     *
     * Example:
     * @code
     * type __int32_t = native.CInt
     * type __darwin_pid_t = __int32_t
     * type pid_t = __darwin_pid_t
     * @endcode
     *
     * Becomes:
     * @code
     * type pid_t = native.CInt
     * @endcode
     *
     */
    void filterDeclarations(const std::string &excludePrefix);

    /**
     * Remove all typedefs that start with given prefix.
     */
    void filterTypeDefs(const std::string &excludePrefix);

    /**
     * Find all typedefs that use oldType and replace it with newType.
     */
    void replaceTypeInTypeDefs(Type *oldType, Type *newType);

    /**
     * @return true if given type is used only in typedefs.
     */
    bool typeIsUsedOnlyInTypeDefs(Type *type);

    /**
     * @return true if type is used in one of given declarations.
     */
    template <typename T>
    bool isTypeUsed(const std::vector<T> &declarations, Type *type);

    void setScalaNames();

    bool existsFunctionWithName(std::string functionName);

    template <typename T>
    void filterByPrefix(std::vector<T> &declarations,
                        const std::string &excludePrefix);

    template <typename T>
    void filterByName(std::vector<T> &declarations, const std::string &name);

    template <typename T>
    T getDeclarationWithName(std::vector<T> &declarations,
                             const std::string &name);

    std::string libName;    // name of the library
    std::string linkName;   // name of the library to link with
    std::string objectName; // name of Scala object
    // TODO: free memory
    std::vector<Function *> functions;
    std::vector<TypeDef *> typeDefs;
    std::vector<Struct *> structs;
    std::vector<Union *> unions;
    std::vector<Enum *> enums;
    std::vector<LiteralDefine *> literalDefines;
    std::vector<PossibleVarDefine *> possibleVarDefines;
    std::vector<VarDefine *> varDefines;
    std::vector<Variable *> variables;
    bool generated = false; // generate type defs only once
    std::string packageName;
};

#endif // SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H
