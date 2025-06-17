cmake_minimum_required(VERSION 3.10)
project(3Dengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

# Add source files
set(SOURCES
    Engine/Rendering/Window.cpp
    Engine/Rendering/Shader.cpp
    Engine/Mesh.cpp
)

# Add test executable
add_executable(test_grass test_grass.cpp ${SOURCES})

# Link libraries
target_link_libraries(test_grass ${OPENGL_LIBRARIES} glfw) 