#pragma once

#include <string>
#include <map>

class HeaderManager {
private:
    std::map<std::string, std::string> headers;

public:
    HeaderManager();
    void LoadConfig(std::string path);
    bool IsStandard(std::string path);
    std::string* GetImport(const std::string& include);

};
