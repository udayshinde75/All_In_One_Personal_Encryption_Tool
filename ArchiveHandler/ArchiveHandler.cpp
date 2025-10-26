#include "ArchiveHandler.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <cstdlib>

namespace fs = std::filesystem;

namespace ArchiveHandler {

    // Validates that a path is a directory (not a file)
    bool isValidFolder(const std::string& folderPath) {
        try {
            return fs::exists(folderPath) && fs::is_directory(folderPath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error checking folder: " << e.what() << std::endl;
            return false;
        }
    }

    // Checks if a folder is empty
    bool isFolderEmpty(const std::string& folderPath) {
        try {
            if (!isValidFolder(folderPath)) {
                return false;
            }
            
            auto iterator = fs::directory_iterator(folderPath);
            return iterator == fs::directory_iterator{};
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error checking if folder is empty: " << e.what() << std::endl;
            return false;
        }
    }

    // Gets the size of a folder (recursive)
    size_t getFolderSize(const std::string& folderPath) {
        try {
            if (!isValidFolder(folderPath)) {
                return 0;
            }

            size_t totalSize = 0;
            for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    totalSize += entry.file_size();
                }
            }
            return totalSize;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error calculating folder size: " << e.what() << std::endl;
            return 0;
        }
    }

    // Creates a unique temporary filename for archives
    std::string generateTempArchiveName(const std::string& originalFolderName) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        // Generate random number
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 9999);
        
        // Create unique filename
        std::stringstream ss;
        ss << "temp_archive_" << originalFolderName << "_" << time_t << "_" << dis(gen) << ".tar";
        
        return ss.str();
    }

    // Creates a temporary archive from a folder using tar command
    std::string createArchiveFromFolder(const std::string& folderPath) {
        try {
            // Validate folder path
            if (!isValidFolder(folderPath)) {
                std::cerr << "Error: Invalid folder path: " << folderPath << std::endl;
                return "";
            }

            // Check if folder is empty
            if (isFolderEmpty(folderPath)) {
                std::cerr << "Error: Cannot create archive from empty folder" << std::endl;
                return "";
            }

            // Get folder name for archive naming
            fs::path folder(folderPath);
            std::string folderName = folder.filename().string();
            
            // Generate unique temporary archive name
            std::string archiveName = generateTempArchiveName(folderName);
            std::string tempDir = fs::temp_directory_path().string();
            std::string archivePath = tempDir + "/" + archiveName;

            // Create tar command
            std::string command = "tar -cf \"" + archivePath + "\" -C \"" + 
                                folder.parent_path().string() + "\" \"" + folderName + "\"";

            // Execute tar command
            int result = std::system(command.c_str());
            
            if (result != 0) {
                std::cerr << "Error: Failed to create archive using tar command" << std::endl;
                return "";
            }

            // Verify archive was created
            if (!fs::exists(archivePath)) {
                std::cerr << "Error: Archive file was not created" << std::endl;
                return "";
            }

            std::cout << "Archive created successfully: " << archivePath << std::endl;
            return archivePath;

        } catch (const std::exception& e) {
            std::cerr << "Error creating archive: " << e.what() << std::endl;
            return "";
        }
    }

    // Extracts an archive back to a folder
    bool extractArchiveToFolder(const std::string& archivePath, const std::string& targetFolderPath) {
        try {
            // Validate archive file exists
            if (!fs::exists(archivePath)) {
                std::cerr << "Error: Archive file does not exist: " << archivePath << std::endl;
                return false;
            }

            // Create target directory if it doesn't exist
            fs::path targetPath(targetFolderPath);
            if (!fs::exists(targetPath)) {
                fs::create_directories(targetPath);
            }

            // Create tar extraction command
            std::string command = "tar -xf \"" + archivePath + "\" -C \"" + targetFolderPath + "\"";

            // Execute tar command
            int result = std::system(command.c_str());
            
            if (result != 0) {
                std::cerr << "Error: Failed to extract archive using tar command" << std::endl;
                return false;
            }

            std::cout << "Archive extracted successfully to: " << targetFolderPath << std::endl;
            return true;

        } catch (const std::exception& e) {
            std::cerr << "Error extracting archive: " << e.what() << std::endl;
            return false;
        }
    }
}
