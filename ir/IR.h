#ifndef SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H
#define SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H

#include "Function.h"
#include "Struct.h"
#include "TypeDef.h"
#include "Enum.h"

/**
 * Intermediate representation
 */
class IR {
public:
    explicit IR(std::string libName);

    void addFunction(std::string name,
                     std::vector<Parameter> parameters,
                     std::string, bool isVariadic);

    void addTypeDef(std::string name, std::string type);

    void addEnum(std::string name, std::vector<Enumerator> enumerators);

    void addStruct(std::string name, std::vector<Field> fields, uint64_t typeSize);

    void addUnion(std::string name, std::vector<Field> fields, uint64_t maxSize);

    /**
     * @return true if there are no functions, types,
     *         structs and unions
     */
    bool libObjEmpty() const;

    bool hasEnums() const;

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &s, const IR &ir);

    void generate();

private:

    /**
     * Generates type defs for enums, structs and unions
     */
    void generateTypeDefs();

    /**
     * @return true if helper methods will be generated for this library
     */
    bool hasHelperMethods() const;

    std::string libName;
    std::vector<Function> functions;
    std::vector<TypeDef> typeDefs;
    std::vector<Struct> structs;
    std::vector<Union> unions;
    std::vector<Enum> enums;
    bool generated = false; // generate type defs only once
};


#endif //SCALA_NATIVE_BINDGEN_INTERMEDIATEREPRESENTATION_H
