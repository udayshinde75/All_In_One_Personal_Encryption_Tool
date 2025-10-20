# FileEncryptionDecryptionTool (Prototype)

A minimal C++ CLI scaffold for an encryption/decryption tool. This prototype focuses on user input, path validation, and menu flow. Cryptographic operations will be added in future iterations.

## Prerequisites
- CMake >= 3.10
- A C++17-compatible compiler (e.g., Clang, GCC, MSVC)

## Build
```bash
mkdir build && cd build
cmake ..
make
```

## Run
```bash
./FileEncryptionDecryptionTool
```

## Usage
- Choose an option from the menu:
  - 1: Encrypt File
  - 2: Decrypt File
  - 3: Encrypt Folder
  - 4: Decrypt Folder
  - 5: Exit
- Provide the full path to the file or folder when prompted.
- Enter a password. The prototype masks the password length in the summary; no encryption is performed yet.

## Project Structure
- `main.cpp`: CLI menu, input handling, and path validation.
- `Utils/Utils.hpp` and `Utils/Utils.cpp`: Small utility helpers (e.g., `Utils::pathExists`).
- `CMakeLists.txt`: Build configuration.

## Next Steps
- Implement file/folder encryption and decryption.
- Add password handling best practices (no echo, secure storage in memory).
- Extend error handling and logging.
