#include "LocationManager.h"
#include "../Utils.h"
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/YAMLTraits.h>

template <> struct llvm::yaml::MappingTraits<HeaderEntry> {
    static void mapping(llvm::yaml::IO &io, HeaderEntry &headerEntry) {
        io.mapRequired("path", headerEntry.path);
        io.mapRequired("object", headerEntry.object);
        io.mapOptional("names", headerEntry.names);
    }
};

template <> struct llvm::yaml::MappingTraits<HeaderEntryName> {
    static void mapping(llvm::yaml::IO &io, HeaderEntryName &headerEntryName) {
        // Dynamically look up the available keys when only one key is given.
        //
        // ```yaml
        //  - struct point: Point
        // ```
        if (io.keys().size() == 1) {
            for (auto key : io.keys()) {
                headerEntryName.original = key;
                io.mapRequired(headerEntryName.original.c_str(),
                               headerEntryName.target);
            }
        } else {
            io.mapRequired("original", headerEntryName.original);
            io.mapRequired("target", headerEntryName.target);
        }
    }
};

LLVM_YAML_IS_SEQUENCE_VECTOR(HeaderEntry)
LLVM_YAML_IS_SEQUENCE_VECTOR(HeaderEntryName)

LocationManager::LocationManager(std::string mainHeaderPath)
    : headerEntries(), mainHeaderPath(std::move(mainHeaderPath)) {}

bool LocationManager::loadConfig(const std::string &path) {
    llvm::SmallString<4096> realPath;
    if (llvm::sys::fs::real_path(path, realPath))
        return false;

    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> mb =
        llvm::MemoryBuffer::getFile(realPath);
    if (!mb)
        return false;

    llvm::yaml::Input input(mb->get()->getBuffer());
    input >> headerEntries;
    return true;
}

bool LocationManager::inMainFile(const Location &location) const {
    return location.getPath() == mainHeaderPath;
}

bool LocationManager::isImported(const Location &location) const {
    if (location.getPath().empty()) {
        return false;
    }
    return getHeaderEntryIndex(location) != headerEntries.size();
}

std::vector<HeaderEntry>::size_type
LocationManager::getHeaderEntryIndex(const Location &location) const {
    auto isHeader = [&](const HeaderEntry &headerEntry) {
        if (startsWith(headerEntry.path, "/") &&
            location.getPath() == headerEntry.path)
            return true;
        return endsWith(location.getPath(), "/" + headerEntry.path);
    };
    auto it =
        std::find_if(headerEntries.begin(), headerEntries.end(), isHeader);
    return it - headerEntries.begin();
}

std::string LocationManager::getImportedType(const Location &location,
                                             const std::string &name) const {
    auto index = getHeaderEntryIndex(location);
    if (index == headerEntries.size())
        return name;

    auto headerEntry = headerEntries[index];

    for (auto const &headerEntryName : headerEntry.names) {
        if (headerEntryName.original == name) {
            return headerEntry.object + "." + headerEntryName.target;
        }
    }

    return headerEntry.object + "." +
           handleReservedWords(replaceChar(name, " ", "_"));
}
