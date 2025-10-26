#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <string>
#include <vector>

// FileHandler namespace - provides file I/O operations for the encryption tool
// All operations are performed in binary mode to handle any file type
namespace FileHandler {

    // Reads a file from disk into memory as binary data
    // Handles files of any type and size
    bool readFile(const std::string& filePath, std::vector<char>& data);

    // Writes binary data from memory to a file on disk
    // Creates or overwrites the target file
    bool writeFile(const std::string& filePath, const std::vector<char>& data);

    // Generates appropriate output filename based on operation type
    // For encryption: adds .enc extension to original filename
    // For decryption: removes .enc extension from filename
    std::string generateOutputFileName(const std::string& inputPath, bool encrypt);
}

#endif
