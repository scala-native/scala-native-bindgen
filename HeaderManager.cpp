#include "HeaderManager.h"

#include "clang/Tooling/Tooling.h"

#include <fstream>

HeaderManager::HeaderManager(): headers() {

}

void HeaderManager::LoadConfig(const std::string& path){
    std::ifstream input(path);

    for(std::string line; getline( input, line );){
        size_t f = line.find("=");
        if(f != line.npos){
            std::string header = line.substr(0, f);
            std::string import = line.substr(f+1, line.npos);
            headers[header] = import;
        }
    }

}

bool HeaderManager::IsStandard(const std::string& path){
    return !!headers.count(path);
}


std::string* HeaderManager::GetImport(const std::string& include){
    return nullptr;
}
