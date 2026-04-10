################################################################################
# MM - Modern Web API Project
# CMakeLists.txt - Build Configuration with Static Linking
################################################################################

# Require CMake 3.14 or higher for FetchContent support
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Define the project
project(
    MM
    VERSION 1.0.0
    DESCRIPTION "A modern C++ Web API using Crow and nlohmann_json"
    LANGUAGES CXX
)

################################################################################
# CMake Policy Configuration
################################################################################

# Set CMP0135 to NEW to suppress timestamp warning with FetchContent
# This policy was introduced in CMake 3.24 to handle downloaded file timestamps
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

################################################################################
# C++ Standard Configuration
################################################################################

# Set C++17 standard (required for underlying frameworks)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# Static Linking Configuration
# Force all dependencies to be built as static libraries
# This ensures everything is bundled into the final executable
################################################################################

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

################################################################################
# FetchContent Setup
# Automatically download and integrate external dependencies using Git
################################################################################

include(FetchContent)

# Enable tracking of fetched content (useful for debugging)
set(FETCHCONTENT_QUIET FALSE)

################################################################################
# Dependency 1: nlohmann_json
# Modern C++ JSON library for JSON serialization/deserialization
# URL: https://github.com/nlohmann/json
################################################################################

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

################################################################################
# Dependency 2: Asio
# Asio is a cross-platform C++ library for network and low-level I/O programming
# Required dependency for Crow
# URL: https://github.com/chriskohlhoff/asio
################################################################################

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-28-0
)

FetchContent_MakeAvailable(asio)

# Set ASIO_INCLUDE_DIR explicitly for Crow to find it
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "" FORCE)

################################################################################
# Dependency 3: Crow
# A modern C++ web framework for building REST APIs
# URL: https://github.com/CrowCpp/Crow
################################################################################

# Disable Crow examples and tests to avoid unnecessary builds
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0
)

FetchContent_MakeAvailable(Crow)

################################################################################
# Dependency 3: cpr
# Modern C++ HTTP client wrapper over libcurl
# URL: https://github.com/libcpr/cpr
################################################################################

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

FetchContent_MakeAvailable(cpr)

################################################################################
# Dependency 4: GoogleTest
# Unit testing framework for C++
# URL: https://github.com/google/googletest
################################################################################

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# Suppress GoogleTest warnings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

################################################################################
# Main Executable Configuration
################################################################################

# Define the main executable
add_executable(MM src/main.cpp)

# Specify C++ standard for the target (explicit declaration)
set_target_properties(MM PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

################################################################################
# Link Main Executable Dependencies
# Link the main executable against all required libraries
################################################################################

target_link_libraries(MM
    PRIVATE
        Crow::Crow
        nlohmann_json::nlohmann_json
        cpr::cpr
)

################################################################################
# Include Directories for Main Executable
################################################################################

target_include_directories(MM
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

################################################################################
# Compiler Flags (Optional - for Production Quality)
################################################################################

# Enable stricter compiler warnings for better code quality
if(MSVC)
    # Microsoft Visual C++ compiler flags
    target_compile_options(MM PRIVATE /W4 /WX)
else()
    # GCC/Clang compiler flags
    target_compile_options(MM PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

################################################################################
# Testing Configuration
################################################################################

# Enable testing for this project
enable_testing()

# Define the test executable
add_executable(MM_tests tests/test_main.cpp)

# Specify C++ standard for the test target
set_target_properties(MM_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

################################################################################
# Link Test Executable Dependencies
# Link the test executable against all required libraries
################################################################################

target_link_libraries(MM_tests
    PRIVATE
        GTest::gtest_main
        nlohmann_json::nlohmann_json
        Crow::Crow
        cpr::cpr
)

################################################################################
# Include Directories for Test Executable
################################################################################

target_include_directories(MM_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

################################################################################
# Register Tests
################################################################################

# Register the test with CTest
add_test(
    NAME BasicTests
    COMMAND MM_tests
)

################################################################################
# Build Output Configuration
################################################################################

# Inform user about the build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Main executable: MM_Backend")
message(STATUS "Test executable: MM_tests")

################################################################################
# Summary
################################################################################

message(STATUS "========== MM Project Configuration ==========")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "CMake Policy CMP0135: NEW (timestamp handling)")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Dependencies:")
message(STATUS "  - nlohmann_json v3.11.3")
message(STATUS "  - Crow v1.2.0")
message(STATUS "  - cpr v1.10.5")
message(STATUS "  - googletest v1.14.0")
message(STATUS "Testing: ENABLED")
message(STATUS "=============================================")

################################################################################
# Windows Socket Libraries
# Link Windows socket and cryptographic libraries required by Asio and OpenSSL
# bcrypt and crypt32 are needed for cryptographic operations
################################################################################

if(WIN32)
    target_link_libraries(MM PRIVATE ws2_32 mswsock bcrypt crypt32)
    target_link_libraries(MM_tests PRIVATE ws2_32 mswsock bcrypt crypt32)
endif()
