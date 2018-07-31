#pragma once

#include "ir/IR.h"
#include <clang/Tooling/Tooling.h>

class TypeTranslator {
  public:
    TypeTranslator(clang::ASTContext *ctx, IR &ir);

    /**
     * @brief Translate the qualified type from c to a scala type
     * @param tpe The type to translate
     * @return the type translated or nullptr if type is function type.
     */
    std::shared_ptr<Type> translate(const clang::QualType &tpe);

    std::string getTypeFromTypeMap(std::string cType);

    std::shared_ptr<TypeDef> addUnionDefinition(clang::RecordDecl *record,
                                                std::string name);

    std::shared_ptr<TypeDef> addStructDefinition(clang::RecordDecl *record,
                                                 std::string name);

    std::shared_ptr<Location> getLocation(clang::Decl *decl);

  private:
    clang::ASTContext *ctx;
    IR &ir;
    int anonymousStructId = 0;
    int anonymousUnionId = 0;

    /**
     * Primitive types
     */
    std::map<std::string, std::string> typeMap;

    std::shared_ptr<Type>
    translateStructOrUnionOrEnum(const clang::QualType &qtpe);

    std::shared_ptr<Type> translateStructOrUnion(const clang::QualType &qtpe);

    std::shared_ptr<Type> translateFunctionPointer(const clang::QualType &qtpe);

    std::shared_ptr<Type> translatePointer(const clang::QualType &pointee);

    std::shared_ptr<Type>
    translateConstantArray(const clang::ConstantArrayType *ar);

    std::shared_ptr<Type> translateEnum(const clang::QualType &type);
};
