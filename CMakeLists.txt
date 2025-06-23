cmake_minimum_required(VERSION 3.10)
project(pmake LANGUAGES C)

# Use strict C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

# Output binary into ./bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Gather source files from src/
file(GLOB PMAKE_SOURCES "src/*.c")

# Define the executable
add_executable(pmake ${PMAKE_SOURCES})

# Include headers from ./include/
target_include_directories(pmake PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Compiler flags
target_compile_options(pmake PRIVATE -Wall -Wextra)