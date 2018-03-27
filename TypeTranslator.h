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
	std::string Translate(const clang::QualType& tpe);
	void AddTranslation(std::string from, std::string to);
};