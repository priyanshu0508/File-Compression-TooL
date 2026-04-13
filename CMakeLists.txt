cmake_minimum_required(VERSION 3.14)

project(FileCompressor
    VERSION 1.0.0
    DESCRIPTION "Advanced File Compression Tool"
    LANGUAGES CXX
)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include FetchContent for dependencies
include(FetchContent)

# 1. cxxopts for CLI parsing
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.1.1
)

# 2. fmt for safe, fast formatting
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.1.1
)

# 3. indicators for terminal progress bars
FetchContent_Declare(
    indicators
    GIT_REPOSITORY https://github.com/p-ranav/indicators.git
    GIT_TAG        v2.3
)

# 4. GoogleTest for unit testing
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows MSVC compatibility, we set this
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cxxopts fmt indicators googletest)

# Subdirectories
add_subdirectory(src)

# Testing (enable if BUILD_TESTING is ON or by default for now)
enable_testing()
add_subdirectory(tests)
