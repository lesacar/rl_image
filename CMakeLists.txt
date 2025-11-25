cmake_minimum_required(VERSION 3.16)
project(rl_image)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include FetchContent module
include(FetchContent)

# Fetch raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

# Set raylib to build as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build raylib as static library")

# Populate raylib
FetchContent_MakeAvailable(raylib)

# Add executable
add_executable(rl_image src/main.cpp)

# Link raylib to our executable
target_link_libraries(rl_image PRIVATE raylib)