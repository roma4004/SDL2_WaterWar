# Path to the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Existing CMake setup
cmake_minimum_required(VERSION 3.10)
project(SDL2_WaterWar VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Specify the output directory for binaries 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define SDL_JOYSTICK_DINPUT 
add_definitions(-DSDL_JOYSTICK_DINPUT)

# Include the command that downloads libraries
include(FetchContent)

# Define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

# Add SDL2 dependency
find_package(SDL2 QUIET)

if (NOT SDL2_FOUND)
    include_dependency(SDL2 https://github.com/libsdl-org/SDL.git release-2.30.8)
    FetchContent_GetProperties(SDL2)
    set(SDL2_INCLUDE_DIRS ${SDL2_SOURCE_DIR}/include) 
    set(SDL2_LIBRARIES ${SDL2_BINARY_DIR}/libSDL2.so) 
else()
    message(STATUS "Using local SDL2")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/Renderer.cpp
    src/Game.cpp
    src/GameTable.cpp
    src/Boat.cpp
    src/EnemyFieldShot.cpp
)

# Create executable
add_executable(SDL2_WaterWar ${SOURCES})

# Set the include directory
target_include_directories(
    SDL2_WaterWar
    PUBLIC
    ${SDL2_INCLUDE_DIRS}
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared"
    "${CMAKE_SOURCE_DIR}/include"
)

# Link all libraries to the project
target_link_libraries(
    SDL2_WaterWar
    PUBLIC
    SDL2::SDL2main
    SDL2::SDL2
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/dinput8.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/dxguid.lib"
)
