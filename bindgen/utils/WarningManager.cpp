#include "WarningManager.h"

#include <llvm/Support/raw_ostream.h>

void WarningManager::printWarning(std::string warning) {
    if (printedWarnings.find(warning) == printedWarnings.end()) {
        llvm::errs() << warning << "\n";
        llvm::errs().flush();
        printedWarnings.insert(warning);
    }
}
