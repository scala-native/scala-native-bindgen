#include "FunctionPointerType.h"
#include <sstream>

FunctionPointerType::FunctionPointerType(
    std::shared_ptr<Type> returnType,
    const std::vector<std::shared_ptr<Type>> &parametersTypes, bool isVariadic)
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

bool FunctionPointerType::usesType(std::shared_ptr<Type> type) const {
    if (this == type.get()) {
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

FunctionPointerType::~FunctionPointerType() {}
