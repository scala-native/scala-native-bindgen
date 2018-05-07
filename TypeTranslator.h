#pragma once

#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Tooling/Tooling.h"

class TypeTranslator {
private:
	clang::ASTContext* ctx;
	std::map<std::string, std::string> typeMap;

public:
	explicit TypeTranslator(clang::ASTContext* ctx);
	void AddTranslation(std::string from, std::string to);

    std::string Translate(const clang::QualType& tpe);
    std::string TranslateFunctionPointer(const clang::QualType& qtpe);
    std::string TranslatePointer(const clang::PointerType* ptr);
    std::string TranslateStructOrUnion(const clang::QualType& qtpe);
    std::string TranslateEnum(const clang::QualType& qtpe);
    std::string TranslateConstantArray(const clang::ConstantArrayType* ar);
};
