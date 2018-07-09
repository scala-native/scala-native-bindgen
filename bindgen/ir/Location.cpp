#include "Location.h"

Location::Location(std::string path, int lineNumber)
    : path(std::move(path)), lineNumber(lineNumber) {}

std::string Location::getPath() const { return path; }

int Location::getLineNumber() const { return lineNumber; }
