#include "Encryption.hpp"
#include "../FileHandler/FileHandler.hpp"
#include <filesystem>
#include <iostream>
#include <cstring>
#include <stdexcept>

namespace fs = std::filesystem;

namespace Encryption {

    // Constructor - initializes encryptor with user's password
    Encryptor::Encryptor(const std::string& password) : password(password) {}

    // Generates encryption key from password with additional entropy
    // Creates key of specified length by repeating password and applying XOR operations
    // This ensures same password produces different keys for different data lengths
    std::vector<char> Encryptor::generateKey(const std::string& password, size_t length) {
        std::vector<char> key(length);
        
        // Generate key by repeating password and adding position-based entropy
        for (size_t i = 0; i < length; ++i) {
            key[i] = password[i % password.length()] ^ (i % 256);
        }
        
        return key;
    }

    // Performs XOR encryption/decryption on data
    // XOR is symmetric - same operation encrypts and decrypts
    // Each byte is XORed with corresponding key byte, key wraps around if shorter
    void Encryptor::xorEncrypt(std::vector<char>& data, const std::vector<char>& key) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] ^= key[i % key.size()];
        }
    }

    // Encrypts raw binary data using password-derived key
    // Creates copy of input data, generates key, and applies XOR encryption
    std::vector<char> Encryptor::encryptData(const std::vector<char>& data) {
        std::vector<char> encrypted = data;
        std::vector<char> key = generateKey(password, data.size());
        xorEncrypt(encrypted, key);
        return encrypted;
    }

    // Decrypts raw binary data using password-derived key
    // Creates copy of encrypted data, generates key, and applies XOR decryption
    std::vector<char> Encryptor::decryptData(const std::vector<char>& encryptedData) {
        std::vector<char> decrypted = encryptedData;
        std::vector<char> key = generateKey(password, encryptedData.size());
        xorEncrypt(decrypted, key);
        return decrypted;
    }

    // Serializes metadata structure to binary format for encryption
    // Format: [filename_length][filename][extension_length][extension][content_size]
    std::vector<char> serializeMetadata(const FileMetadata& metadata) {
        std::vector<char> result;
        
        // Store original filename length (4 bytes)
        uint32_t filenameLength = metadata.originalFilename.length();
        result.insert(result.end(), reinterpret_cast<char*>(&filenameLength), 
                     reinterpret_cast<char*>(&filenameLength) + sizeof(uint32_t));
        
        // Store original filename
        result.insert(result.end(), metadata.originalFilename.begin(), metadata.originalFilename.end());
        
        // Store extension length (4 bytes)
        uint32_t extensionLength = metadata.extension.length();
        result.insert(result.end(), reinterpret_cast<char*>(&extensionLength), 
                     reinterpret_cast<char*>(&extensionLength) + sizeof(uint32_t));
        
        // Store extension
        result.insert(result.end(), metadata.extension.begin(), metadata.extension.end());
        
        // Store content size (8 bytes)
        uint64_t contentSize = metadata.contentSize;
        result.insert(result.end(), reinterpret_cast<char*>(&contentSize), 
                     reinterpret_cast<char*>(&contentSize) + sizeof(uint64_t));
        
        return result;
    }

    // Deserializes metadata from binary format with comprehensive validation
    // Converts binary data back to FileMetadata structure
    // Includes validation to prevent crashes from invalid/corrupted data
    FileMetadata deserializeMetadata(const std::vector<char>& data) {
        FileMetadata metadata;
        size_t offset = 0;
        
        // Validate minimum data size
        if (data.size() < sizeof(uint32_t)) {
            throw std::runtime_error("Insufficient data for filename length");
        }
        
        // Read filename length
        uint32_t filenameLength;
        std::memcpy(&filenameLength, data.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        
        // Validate filename length is reasonable
        if (filenameLength > 1000 || offset + filenameLength > data.size()) {
            throw std::runtime_error("Invalid filename length");
        }
        
        // Read filename
        metadata.originalFilename.assign(data.begin() + offset, data.begin() + offset + filenameLength);
        offset += filenameLength;
        
        // Validate we have enough data for extension length
        if (offset + sizeof(uint32_t) > data.size()) {
            throw std::runtime_error("Insufficient data for extension length");
        }
        
        // Read extension length
        uint32_t extensionLength;
        std::memcpy(&extensionLength, data.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        
        // Validate extension length is reasonable
        if (extensionLength > 100 || offset + extensionLength > data.size()) {
            throw std::runtime_error("Invalid extension length");
        }
        
        // Read extension
        metadata.extension.assign(data.begin() + offset, data.begin() + offset + extensionLength);
        offset += extensionLength;
        
        // Validate we have enough data for content size
        if (offset + sizeof(uint64_t) > data.size()) {
            throw std::runtime_error("Insufficient data for content size");
        }
        
        // Read content size
        uint64_t contentSize;
        std::memcpy(&contentSize, data.data() + offset, sizeof(uint64_t));
        metadata.contentSize = contentSize;
        
        return metadata;
    }

    // Encrypts a file and saves it with metadata
    // Reads original file, extracts filename/extension, encrypts metadata and content
    // Saves encrypted file with .enc extension containing all necessary data for decryption
    bool Encryptor::encryptFile(const std::string& inputPath, const std::string& outputPath) {
        // Read original file into memory
        std::vector<char> fileData;
        if (!FileHandler::readFile(inputPath, fileData)) {
            return false;
        }
        
        // Extract filename and extension from path
        fs::path path(inputPath);
        std::string originalFilename = path.filename().string();
        std::string extension = path.extension().string();
        
        // Create metadata structure with file information
        FileMetadata metadata;
        metadata.originalFilename = originalFilename;
        metadata.extension = extension;
        metadata.contentSize = fileData.size();
        
        // Serialize metadata to binary format
        std::vector<char> metadataData = serializeMetadata(metadata);
        
        // Encrypt metadata using password-derived key
        std::vector<char> encryptedMetadata = encryptData(metadataData);
        
        // Encrypt file content using password-derived key
        std::vector<char> encryptedContent = encryptData(fileData);
        
        // Combine encrypted metadata and content into final file format
        std::vector<char> finalData;
        
        // Store metadata size (4 bytes) - needed for decryption
        uint32_t metadataSize = encryptedMetadata.size();
        finalData.insert(finalData.end(), reinterpret_cast<char*>(&metadataSize), 
                        reinterpret_cast<char*>(&metadataSize) + sizeof(uint32_t));
        
        // Store encrypted metadata
        finalData.insert(finalData.end(), encryptedMetadata.begin(), encryptedMetadata.end());
        
        // Store encrypted content
        finalData.insert(finalData.end(), encryptedContent.begin(), encryptedContent.end());
        
        // Write encrypted file to disk
        return FileHandler::writeFile(outputPath, finalData);
    }

    // Decrypts an encrypted file and restores original file
    // Reads encrypted file, decrypts metadata to get original filename/extension
    // Decrypts content and saves with original filename
    // Includes comprehensive validation to prevent crashes from invalid data
    bool Encryptor::decryptFile(const std::string& inputPath, const std::string& outputPath) {
        // Read encrypted file into memory
        std::vector<char> encryptedData;
        if (!FileHandler::readFile(inputPath, encryptedData)) {
            return false;
        }
        
        // Validate minimum file size (must have at least metadata size + some data)
        if (encryptedData.size() < sizeof(uint32_t) + 1) {
            std::cerr << "Error: Invalid encrypted file format - file too small" << std::endl;
            return false;
        }
        
        size_t offset = 0;
        
        // Read metadata size from beginning of file
        uint32_t metadataSize;
        std::memcpy(&metadataSize, encryptedData.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        
        // Validate metadata size is reasonable
        if (metadataSize == 0 || metadataSize > encryptedData.size() - sizeof(uint32_t)) {
            std::cerr << "Error: Invalid encrypted file format - corrupted metadata size" << std::endl;
            return false;
        }
        
        // Validate we have enough data for metadata
        if (offset + metadataSize > encryptedData.size()) {
            std::cerr << "Error: Invalid encrypted file format - insufficient data for metadata" << std::endl;
            return false;
        }
        
        // Extract encrypted metadata
        std::vector<char> encryptedMetadata(encryptedData.begin() + offset, 
                                           encryptedData.begin() + offset + metadataSize);
        offset += metadataSize;
        
        // Decrypt metadata using password-derived key
        std::vector<char> metadataData = decryptData(encryptedMetadata);
        
        // Validate decrypted metadata size
        if (metadataData.size() < sizeof(uint32_t) * 3 + sizeof(uint64_t)) {
            std::cerr << "Error: Invalid password or corrupted file - metadata too small" << std::endl;
            return false;
        }
        
        // Deserialize metadata with error checking
        FileMetadata metadata;
        try {
            metadata = deserializeMetadata(metadataData);
        } catch (...) {
            std::cerr << "Error: Invalid password or corrupted file - cannot parse metadata" << std::endl;
            return false;
        }
        
        // Validate metadata content
        if (metadata.originalFilename.empty() || metadata.contentSize == 0) {
            std::cerr << "Error: Invalid password or corrupted file - invalid metadata content" << std::endl;
            return false;
        }
        
        // Extract encrypted content
        if (offset >= encryptedData.size()) {
            std::cerr << "Error: Invalid encrypted file format - no content data" << std::endl;
            return false;
        }
        
        std::vector<char> encryptedContent(encryptedData.begin() + offset, encryptedData.end());
        
        // Decrypt content using password-derived key
        std::vector<char> decryptedContent = decryptData(encryptedContent);
        
        // Verify content size matches metadata
        if (decryptedContent.size() != metadata.contentSize) {
            std::cerr << "Error: Invalid password or corrupted file - content size mismatch" << std::endl;
            return false;
        }
        
        // Write decrypted content to output file
        return FileHandler::writeFile(outputPath, decryptedContent);
    }
}
