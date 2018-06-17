#include "Define.h"

Define::Define(std::string name) : name(std::move(name)) {}

std::string Define::getName() { return name; }
