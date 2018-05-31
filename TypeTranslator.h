#pragma once

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/Tooling/Tooling.h>

class TypeTranslator {
private:
	clang::ASTContext* ctx;
	std::map<std::string, std::string> typeMap;

public:
	explicit TypeTranslator(clang::ASTContext* ctx);
	void AddTranslation(std::string from, std::string to);

    /**
     * @brief Translate the qualified type from c to a scala type
     * @param tpe The type to translate
     * @param avoid A type to avoid, useful to avoid cyclic definitions inside structs, unions, ...
     * @return the type translated
     */
    std::string Translate(const clang::QualType& tpe, const std::string* = nullptr);
    std::string TranslateFunctionPointer(const clang::QualType& qtpe, const std::string* avoid );
    std::string TranslatePointer(const clang::QualType& pointee, const std::string* avoid);
    std::string TranslateStructOrUnion(const clang::QualType& qtpe);
    std::string TranslateEnum(const clang::QualType& qtpe);
    std::string TranslateConstantArray(const clang::ConstantArrayType* ar, const std::string* avoid );
};
