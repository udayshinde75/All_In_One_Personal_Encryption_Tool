#ifndef ARCHIVEHANDLER_HPP
#define ARCHIVEHANDLER_HPP

#include <string>
#include <vector>

// ArchiveHandler namespace - provides folder archiving operations for the encryption tool
// Creates temporary archives from folders and extracts archives back to folders
namespace ArchiveHandler {

    // Creates a temporary archive from a folder
    // Uses tar format for cross-platform compatibility
    // Returns the path to the created archive file
    std::string createArchiveFromFolder(const std::string& folderPath);

    // Extracts an archive back to a folder
    // Creates the target folder if it doesn't exist
    // Returns true if extraction was successful
    bool extractArchiveToFolder(const std::string& archivePath, const std::string& targetFolderPath);

    // Validates that a path is a directory (not a file)
    // Returns true if the path exists and is a directory
    bool isValidFolder(const std::string& folderPath);

    // Checks if a folder is empty
    // Returns true if folder exists and contains no files/subdirectories
    bool isFolderEmpty(const std::string& folderPath);

    // Gets the size of a folder (recursive)
    // Returns the total size in bytes of all files in the folder
    size_t getFolderSize(const std::string& folderPath);

    // Creates a unique temporary filename for archives
    // Uses timestamp and random number to avoid conflicts
    std::string generateTempArchiveName(const std::string& originalFolderName);
}

#endif
