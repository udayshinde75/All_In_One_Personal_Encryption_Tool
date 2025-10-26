#include <iostream>
#include <filesystem>
#include <string>
#include "Utils/Utils.hpp"
#include "FileHandler/FileHandler.hpp"
#include "Encryption/Encryption.hpp"
#include "ArchiveHandler/ArchiveHandler.hpp"

// FileCrypt - File Encryption/Decryption Tool
// This is the main entry point for a command-line tool that encrypts and decrypts files and folders.
// The tool uses XOR-based encryption with password-derived keys and stores encrypted
// files with metadata to preserve original filenames and extensions.
// For folders, it creates temporary archives and encrypts them using the same process.

using namespace std;
namespace fs = std::filesystem;

// Displays the main menu with available operations
// Options 1-2: File encryption/decryption (implemented)
// Options 3-4: Folder encryption/decryption (implemented using archive creation)
void displayMenu() {
    cout << "=============================\n";
    cout << "   FileCrypt - Encryption Tool\n";
    cout << "=============================\n";
    cout << "1. Encrypt File\n";
    cout << "2. Decrypt File\n";
    cout << "3. Encrypt Folder\n";
    cout << "4. Decrypt Folder\n";
    cout << "5. Exit\n";
    cout << "-----------------------------\n";
    cout << "Enter your choice: ";
}

// Main application loop - handles user input and performs encryption/decryption operations
int main() {
    int choice;        // User's menu selection
    string path;       // File/folder path from user
    string password;   // Encryption password

    // Main loop continues until user chooses to exit
    while (true)
    {
        // Display menu and get user's choice
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer

        // Handle exit option
        if ( choice == 5 )
        {
            cout << "Exiting FileCrypt. Goodbye!\n";
            break;
        }

        // Prompt for appropriate path based on operation type
        if (choice == 1 || choice == 2)
        {
            // File operations (encrypt/decrypt single file)
            cout << "Enter the full path of file: ";
        }
        else if (choice == 3 || choice == 4)
        {
            // Folder operations 
            cout << "Enter the full path of folder: ";
        }
        else
        {
            // Invalid choice - restart loop
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        // Read the file/folder path from user
        getline(cin, path);

        // Validate that the specified path exists on the filesystem
        if (!Utils::pathExists(path)) {
            cout << "❌ Error: The specified path does not exist.\n\n";
            continue;
        }
        
        // Get encryption password from user
        cout << "Enter your password: ";
        getline(cin, password);

        // Display operation summary to user
        cout << "\n----------------------------------------\n";
        cout << "Processing operation: " << choice << endl;
        cout << "Target path: " << path << endl;
        cout << "Password: " << string(password.size(), '*') << endl; // Mask password for display
        cout << "----------------------------------------\n\n";

        // Create encryption engine with user's password
        Encryption::Encryptor encryptor(password);
        bool success = false;    // Track operation success
        string outputPath;       // Path for output file

        // Execute the requested operation
        switch (choice) {
            case 1: // Encrypt File
                // Generate output filename with .enc extension
                outputPath = FileHandler::generateOutputFileName(path, true);
                // Perform encryption operation
                success = encryptor.encryptFile(path, outputPath);
                if (success) {
                    cout << "✅ File encrypted successfully!" << endl;
                    cout << "Encrypted file saved as: " << outputPath << endl;
                } else {
                    cout << "❌ Failed to encrypt file." << endl;
                }
                break;

            case 2: // Decrypt File
                // Generate output filename by removing .enc extension
                outputPath = FileHandler::generateOutputFileName(path, false);
                // Perform decryption operation
                success = encryptor.decryptFile(path, outputPath);
                if (success) {
                    cout << "✅ File decrypted successfully!" << endl;
                    cout << "Decrypted file saved as: " << outputPath << endl;
                } else {
                    cout << "❌ Failed to decrypt file." << endl;
                }
                break;

                case 3: // Encrypt Folder
                {
                    // Validate folder path
                    if (!ArchiveHandler::isValidFolder(path)) {
                        cout << "❌ Error: Invalid folder path or folder does not exist." << endl;
                        break;
                    }
                
                    // Check if folder is empty
                    if (ArchiveHandler::isFolderEmpty(path)) {
                        cout << "❌ Error: Cannot encrypt empty folder." << endl;
                        break;
                    }
                
                    // Get folder size for user info
                    size_t folderSize = ArchiveHandler::getFolderSize(path);
                    cout << "📁 Folder size: " << folderSize << " bytes" << endl;
                
                    // Create temporary archive
                    cout << "📦 Creating archive from folder..." << endl;
                    string archivePath = ArchiveHandler::createArchiveFromFolder(path);
                    if (archivePath.empty()) {
                        cout << "❌ Failed to create archive from folder." << endl;
                        break;
                    }
                
                    // 🟢 Create clean output path beside the original folder
                    fs::path folderPath(path);
                    fs::path parentDir = folderPath.parent_path();
                    std::string folderName = folderPath.filename().string();
                
                    // The encrypted file will have the same name with .enc extension
                    outputPath = (parentDir / (folderName + ".enc")).string();
                
                    cout << "🔒 Encrypting folder..." << endl;
                    success = encryptor.encryptFile(archivePath, outputPath);
                
                    // Clean up temp archive
                    if (fs::exists(archivePath)) {
                        fs::remove(archivePath);
                        cout << "🧹 Cleaned up temporary archive file." << endl;
                    }
                
                    if (success) {
                        cout << "✅ Folder encrypted successfully!" << endl;
                        cout << "Encrypted file saved as: " << outputPath << endl;
                    } else {
                        cout << "❌ Failed to encrypt folder." << endl;
                    }
                
                    break;
                }
                
                
            case 4: // Decrypt Folder
                {
                    // Generate output filename by removing .enc extension
                    outputPath = FileHandler::generateOutputFileName(path, false);
                    
                    // Decrypt the archive file
                    cout << "🔓 Decrypting archive..." << endl;
                    success = encryptor.decryptFile(path, outputPath);
                    
                    if (!success) {
                        cout << "❌ Failed to decrypt folder." << endl;
                        break;
                    }
                    
                    // Extract archive to folder
                    cout << "📦 Extracting archive to folder..." << endl;
                    string extractPath = outputPath + "_extracted";
                    success = ArchiveHandler::extractArchiveToFolder(outputPath, extractPath);
                    
                    // Clean up temporary archive file
                    if (fs::exists(outputPath)) {
                        fs::remove(outputPath);
                        cout << "🧹 Cleaned up temporary archive file." << endl;
                    }
                    
                    if (success) {
                        cout << "✅ Folder decrypted successfully!" << endl;
                        cout << "Decrypted folder saved as: " << extractPath << endl;
                    } else {
                        cout << "❌ Failed to extract folder from archive." << endl;
                    }
                    break;

                }
                
            default:
                // This should not happen due to earlier validation, but included for safety
                cout << "❌ Invalid choice." << endl;
                break;
        }
        
        cout << endl;
        
    }
    
}