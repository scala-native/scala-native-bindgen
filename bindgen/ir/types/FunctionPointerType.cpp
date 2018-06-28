#include "FunctionPointerType.h"
#include <sstream>

FunctionPointerType::FunctionPointerType(
    Type *returnType, const std::vector<Type *> &parametersTypes,
    bool isVariadic)
    : returnType(returnType), parametersTypes(parametersTypes),
      isVariadic(isVariadic) {}

std::string FunctionPointerType::str() const {
    std::stringstream ss;
    ss << "native.CFunctionPtr" << parametersTypes.size() << "[";

    for (const auto &parameterType : parametersTypes) {
        ss << parameterType->str() << ", ";
    }

    if (isVariadic) {
        ss << "native.CVararg, ";
    }
    ss << returnType->str() << "]";
    return ss.str();
}

bool FunctionPointerType::usesType(Type *type) const {
    if (this == type) {
        return true;
    }
    if (returnType == type) {
        return true;
    }

    for (auto parameterType : parametersTypes) {
        if (parameterType == type) {
            return true;
        }
    }
    return false;
}

FunctionPointerType::~FunctionPointerType() {
    if (returnType->canBeDeallocated()) {
        delete returnType;
    }
    for (const auto &parameterType : parametersTypes) {
        if (parameterType->canBeDeallocated()) {
            delete parameterType;
        }
    }
}
