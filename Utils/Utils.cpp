#include "Utils.hpp"
#include <filesystem>

// Implementation of minimal filesystem utilities.

namespace fs = std::filesystem;

namespace Utils {
    bool pathExists(const std::string& path) {
        // std::filesystem::exists resolves both files and directories.
        return fs::exists(path);
    }
}
