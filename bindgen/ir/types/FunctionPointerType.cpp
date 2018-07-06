#include "FunctionPointerType.h"
#include "../../Utils.h"
#include <sstream>

FunctionPointerType::FunctionPointerType(
    std::shared_ptr<Type> returnType,
    const std::vector<std::shared_ptr<Type>> &parametersTypes, bool isVariadic)
    : returnType(std::move(returnType)), parametersTypes(parametersTypes),
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

bool FunctionPointerType::usesType(const std::shared_ptr<Type> &type) const {
    if (*returnType == *type) {
        return true;
    }

    for (const auto &parameterType : parametersTypes) {
        if (*parameterType == *type) {
            return true;
        }
    }
    return false;
}

bool FunctionPointerType::operator==(const Type &other) const {
    if (this == &other) {
        return true;
    }
    if (isInstanceOf<const FunctionPointerType>(&other)) {
        auto *functionPointerType =
            dynamic_cast<const FunctionPointerType *>(&other);
        if (isVariadic != functionPointerType->isVariadic) {
            return false;
        }
        if (*returnType != *functionPointerType->returnType) {
            return false;
        }
        if (parametersTypes.size() !=
            functionPointerType->parametersTypes.size()) {
            return false;
        }
        for (size_t i = 0; i < parametersTypes.size(); i++) {
            if (*parametersTypes[i] !=
                *functionPointerType->parametersTypes[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}
