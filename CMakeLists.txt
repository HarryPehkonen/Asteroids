# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Asteroids VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Define source files (removed unused files)
set(SOURCES
    src/main.cpp
    src/GameObject.cpp
    src/Ship.cpp
    src/Asteroid.cpp
    src/Bullet.cpp
    src/GameState.cpp
)

# Create a library target for the game code (excluding main.cpp)
add_library(asteroids_lib
    src/GameObject.cpp
    src/Ship.cpp
    src/Asteroid.cpp
    src/Bullet.cpp
    src/GameState.cpp
)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link SFML to the library
target_link_libraries(asteroids_lib PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)

# Link the executable with our library
target_link_libraries(${PROJECT_NAME} PRIVATE
    asteroids_lib
)

# Include directories
target_include_directories(asteroids_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Copy assets to build directory
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Enable testing
enable_testing()

# Add test directory if it exists
if(EXISTS ${PROJECT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()

# Set compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(asteroids_lib PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(asteroids_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
