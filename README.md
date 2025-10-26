This project is made to understand C++ concepts

# FileCrypt - File Encryption/Decryption Tool

A comprehensive C++ command-line tool for encrypting and decrypting files using XOR-based encryption with password-derived keys. The tool preserves original filenames and extensions by storing metadata within encrypted files.

## Features

- **File Encryption/Decryption**: Encrypt any file type while preserving original filename and extension
- **Folder Encryption/Decryption**: Encrypt entire folders by creating archives and encrypting them
- **Password-Based Security**: Uses XOR encryption with password-derived keys
- **Metadata Preservation**: Stores original filename, extension, and content size for perfect reconstruction
- **Archive Management**: Automatic creation and cleanup of temporary archives for folder operations
- **Error Handling**: Comprehensive validation prevents crashes from invalid passwords or corrupted files
- **Cross-Platform**: Works on any system with C++17 support and tar command availability

## Encryption Algorithm

The tool implements a custom encryption format that stores:

1. **Metadata Size** (4 bytes) - Size of encrypted metadata
2. **Encrypted Metadata** - Contains original filename, extension, and content size
3. **Encrypted Content** - The actual file content

### Encryption Process:
1. Read original file into memory
2. Extract filename and extension
3. Create metadata structure
4. Encrypt metadata and content using password-derived XOR keys
5. Save to `.enc` file with proper format

### Decryption Process:
1. Read encrypted file
2. Decrypt metadata to get original filename/extension
3. Decrypt content
4. Save with original filename

## Prerequisites

- CMake >= 3.10
- A C++17-compatible compiler (e.g., Clang, GCC, MSVC)

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

Run the executable:
```bash
./FileEncryptionDecryptionTool
```

### Menu Options:
- **1. Encrypt File** - Encrypt a single file (creates `.enc` file)
- **2. Decrypt File** - Decrypt a `.enc` file (restores original file)
- **3. Encrypt Folder** - Encrypt entire folder (creates archive, encrypts it)
- **4. Decrypt Folder** - Decrypt folder archive (decrypts, extracts to folder)
- **5. Exit** - Exit the application

### Example Usage:

**File Encryption/Decryption:**
1. Choose option 1 to encrypt a file
2. Enter the full path to your file
3. Enter your password
4. The encrypted file will be saved as `filename.enc`
5. To decrypt, choose option 2 and select the `.enc` file
6. Enter the same password to restore the original file

**Folder Encryption/Decryption:**
1. Choose option 3 to encrypt a folder
2. Enter the full path to your folder
3. Enter your password
4. The tool creates a temporary archive, encrypts it, and saves as `foldername.tar.enc`
5. To decrypt, choose option 4 and select the `.enc` file
6. Enter the same password to restore the original folder structure

## Project Structure

```
EncryptionTool/
├── main.cpp                 # Main application with CLI interface
├── FileHandler/             # File I/O operations
│   ├── FileHandler.hpp     # Header for file operations
│   └── FileHandler.cpp     # Implementation of file read/write functions
├── Encryption/              # Core encryption functionality
│   ├── Encryption.hpp      # Header for encryption classes and functions
│   └── Encryption.cpp      # Implementation of XOR encryption and metadata handling
├── ArchiveHandler/          # Folder archiving operations
│   ├── ArchiveHandler.hpp  # Header for archive creation/extraction
│   └── ArchiveHandler.cpp  # Implementation using tar commands
├── Utils/                   # Utility functions
│   ├── Utils.hpp           # Header for utility functions
│   └── Utils.cpp           # Implementation of path validation
└── CMakeLists.txt          # Build configuration
```

## Security Notes

- **XOR Encryption**: This tool uses XOR-based encryption, which is suitable for basic file protection but not for high-security applications
- **Password Storage**: Passwords are not stored anywhere and must be remembered
- **File Integrity**: The tool includes validation to detect corrupted files and wrong passwords
- **Error Handling**: Comprehensive error checking prevents crashes from invalid input

## Error Handling

The tool includes extensive error handling for:
- Invalid file/folder paths
- Wrong passwords
- Corrupted encrypted files
- Empty folders (cannot encrypt empty folders)
- Archive creation/extraction failures
- Temporary file cleanup

Error messages are displayed to help users understand what went wrong.

## Future Enhancements

- Stronger encryption algorithms (AES)
- Password strength validation
- Batch processing capabilities
- GUI interface
- Progress indicators for large folder operations
- Compression options for archives

