#include "SourceLocation.h"

SourceLocation::SourceLocation(bool isMainFile) : mainFile(isMainFile) {}

bool SourceLocation::isMainFile() const { return mainFile; }
