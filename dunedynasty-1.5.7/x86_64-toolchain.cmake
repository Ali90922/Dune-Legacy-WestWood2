# x86_64-toolchain.cmake
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compiler
SET(CMAKE_C_COMPILER x86_64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER x86_64-linux-gnu-g++)

# Where to look for the target environment
SET(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-gnu)

# Search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
