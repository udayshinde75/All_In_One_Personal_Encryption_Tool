cmake_minimum_required(VERSION 3.10)
project(FileEncryptionDecryptionTool)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileEncryptionDecryptionTool
    main.cpp
    Utils/Utils.cpp
)
