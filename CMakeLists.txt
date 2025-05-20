cmake_minimum_required(VERSION 3.10)
project(HybridCacheSimulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/CacheSimulator.cpp
)

# Create executable
add_executable(cache_simulator ${SOURCES})

# Add include directories
target_include_directories(cache_simulator
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cache_simulator PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Add GoogleTest
option(BUILD_TESTS "Build test cases" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif() 