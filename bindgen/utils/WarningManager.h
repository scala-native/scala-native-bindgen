#ifndef SCALA_NATIVE_BINDGEN_WARNINGMANAGER_H
#define SCALA_NATIVE_BINDGEN_WARNINGMANAGER_H

#include <string>
#include <unordered_set>

/**
 * Ignores duplicate warnings
 */
class WarningManager {
  public:
    void printWarning(std::string warning);

  private:
    std::unordered_set<std::string> printedWarnings;
};

#endif // SCALA_NATIVE_BINDGEN_WARNINGMANAGER_H
