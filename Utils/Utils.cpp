#include "Utils.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace Utils {
    // Checks if a given file or directory path exists on the filesystem
    // Uses std::filesystem::exists which works for both files and directories
    // Handles both absolute and relative paths
    bool pathExists(const std::string& path) {
        return fs::exists(path);
    }
}
