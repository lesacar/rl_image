cmake_minimum_required(VERSION 4.2) # needed for VS 2026
project(rl_image)

set(INC_DIR "src/include/")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

# Raylib options
set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
set(SUPPORT_GIF_RECORDING OFF CACHE BOOL "" FORCE)
set(SUPPORT_SCREEN_CAPTURE OFF CACHE BOOL "" FORCE)
set(SUPPORT_AUTOMATION_EVENTS OFF CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_BMP ON CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_TGA ON CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_JPG ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_executable(rl_image src/main.cpp src/engine.cpp src/timer.cpp src/window.cpp src/wimage.cpp)

# need C++26 for <debugging>, and lvalue references in make_format_args
if(MSVC)
    # c++latest should be C++26
    target_compile_options(rl_image PRIVATE
        /arch:AVX2 /W4 /std:c++latest
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Debug>:/Od /DEBUG>
    )
elseif(WIN32)
    message( FATAL_ERROR "Unsupported compiler for WIN32, consider MSVC. Aborting..." )
else()
    # c++2c should be c++26
    target_compile_options(rl_image PRIVATE
        -mavx2 -Wall -Wextra -std=c++2c
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Debug>:-O0 -g>
    )
endif()

target_include_directories(rl_image PRIVATE ${INC_DIR})

target_link_libraries(rl_image PRIVATE raylib)
