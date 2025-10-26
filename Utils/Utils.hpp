#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Utils namespace - provides lightweight utility functions for the encryption tool
// Currently includes filesystem path validation functionality
namespace Utils {
    // Checks if a given file or directory path exists on the filesystem
    // Works with both absolute and relative paths, handles files and directories
    // No permission checks are performed - only existence check
    bool pathExists(const std::string& path);
}

#endif
