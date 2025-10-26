#include "FileHandler.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace FileHandler {

    // Reads a file from disk into memory as binary data
    // Opens file in binary mode, determines size, and reads entire content
    bool readFile(const std::string& filePath, std::vector<char>& data) {
        // Open file in binary mode for reading
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return false;
        }

        // Determine file size by seeking to end
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg); // Reset to beginning

        // Resize vector to accommodate file size
        data.resize(fileSize);
        
        // Read entire file content into vector
        file.read(data.data(), fileSize);
        
        // Check if read operation was successful
        if (file.fail()) {
            std::cerr << "Error: Failed to read file " << filePath << std::endl;
            return false;
        }

        file.close();
        return true;
    }

    // Writes binary data from memory to a file on disk
    // Creates or overwrites the target file in binary mode
    bool writeFile(const std::string& filePath, const std::vector<char>& data) {
        // Open file in binary mode for writing (creates or overwrites)
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filePath << std::endl;
            return false;
        }

        // Write all data to file
        file.write(data.data(), data.size());
        
        // Check if write operation was successful
        if (file.fail()) {
            std::cerr << "Error: Failed to write file " << filePath << std::endl;
            return false;
        }

        file.close();
        return true;
    }

    // Generates appropriate output filename based on operation type
    // For encryption: appends .enc extension to original filename
    // For decryption: removes .enc extension if present
    std::string generateOutputFileName(const std::string& inputPath, bool encrypt) {
        // Parse the input path using filesystem utilities
        fs::path path(inputPath);
        
        if (encrypt) {
            // For encryption: append .enc extension to original filename
            return path.string() + ".enc";
        } else {
            // For decryption: remove .enc extension if present
            std::string filename = path.filename().string();
            if (filename.length() > 4 && filename.substr(filename.length() - 4) == ".enc") {
                // Remove .enc extension and reconstruct path
                return path.parent_path().string() + "/" + filename.substr(0, filename.length() - 4);
            }
            // Return original path if not a .enc file
            return inputPath;
        }
    }
}
