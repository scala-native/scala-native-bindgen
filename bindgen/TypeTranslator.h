#pragma once

#include "ir/IR.h"
#include <clang/Tooling/Tooling.h>

class TypeTranslator {
  public:
    TypeTranslator(clang::ASTContext *ctx, IR &ir);

    /**
     * @brief Translate the qualified type from c to a scala type
     * @param tpe The type to translate
     * @param avoid A type to avoid, useful to avoid cyclic definitions inside
     * structs, unions, ...
     * @return the type translated
     */
    Type *translate(const clang::QualType &tpe, const std::string * = nullptr);

    void addAlias(std::string cName, Type *type);

    std::string getTypeFromTypeMap(std::string cType);

  private:
    clang::ASTContext *ctx;
    IR &ir;

    /**
     * Primitive types
     */
    std::map<std::string, std::string> typeMap;

    /**
     * Maps C struct, union or enum name to Type alias
     */
    std::map<std::string, Type *> aliasesMap;

    Type *translateStructOrUnionOrEnum(const clang::QualType &qtpe);

    Type *translateStructOrUnion(const clang::QualType &qtpe);

    Type *translateFunctionPointer(const clang::QualType &qtpe,
                                   const std::string *avoid);

    Type *translatePointer(const clang::QualType &pointee,
                           const std::string *avoid);

    Type *translateConstantArray(const clang::ConstantArrayType *ar,
                                 const std::string *avoid);
};
