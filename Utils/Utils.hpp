#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Lightweight utility helpers used by the CLI.
// Keep this header minimal; prefer declarations here and definitions in Utils.cpp.

namespace Utils {
    // Returns true if the given file or directory exists on the filesystem.
    // Accepts absolute or relative paths. No permission checks are performed.
    bool pathExists(const std::string& path);
}

#endif
