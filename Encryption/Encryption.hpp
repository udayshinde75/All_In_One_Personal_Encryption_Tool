#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>
#include <vector>

// Encryption namespace - provides core encryption/decryption functionality
// Uses XOR-based encryption with password-derived keys
// Stores encrypted files with metadata to preserve original filenames and extensions
namespace Encryption {

    // Main encryption class implementing XOR-based encryption
    // Uses password-derived keys for symmetric encryption/decryption
    class Encryptor {
    private:
        std::string password;  // User-provided password for encryption/decryption
        
        // Generates encryption key from password with additional entropy
        // Creates key of specified length by repeating password and applying XOR operations
        std::vector<char> generateKey(const std::string& password, size_t length);
        
        // Performs XOR encryption/decryption on data
        // XOR is symmetric - same operation encrypts and decrypts
        void xorEncrypt(std::vector<char>& data, const std::vector<char>& key);
        
    public:
        // Constructor - initializes encryptor with user's password
        Encryptor(const std::string& password);
        
        // Encrypts a file and saves it with metadata
        // Reads file, extracts filename/extension, encrypts metadata and content
        bool encryptFile(const std::string& inputPath, const std::string& outputPath);
        
        // Decrypts an encrypted file and restores original file
        // Reads encrypted file, decrypts metadata, decrypts content, saves with original name
        bool decryptFile(const std::string& inputPath, const std::string& outputPath);
        
        // Encrypts raw binary data using password-derived key
        std::vector<char> encryptData(const std::vector<char>& data);
        
        // Decrypts raw binary data using password-derived key
        std::vector<char> decryptData(const std::vector<char>& encryptedData);
    };

    // Structure to hold file metadata for encryption format
    // Contains information needed to reconstruct original file during decryption
    struct FileMetadata {
        std::string originalFilename;  // Original filename without path
        std::string extension;          // File extension (including dot)
        size_t contentSize;            // Size of original file content
    };
    
    // Serializes metadata structure to binary format for encryption
    // Converts FileMetadata to binary data suitable for encryption and storage
    std::vector<char> serializeMetadata(const FileMetadata& metadata);
    
    // Deserializes metadata from binary format
    // Converts binary data back to FileMetadata structure with validation
    // Throws exceptions for invalid data to prevent crashes
    FileMetadata deserializeMetadata(const std::vector<char>& data);
}

#endif
