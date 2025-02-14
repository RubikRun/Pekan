cmake_minimum_required(VERSION 3.6...3.31)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set avaiable configurations to Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Create project Pekan
project(Pekan)

# Add Demo subdirectory
add_subdirectory(src/Demo)

# Add GLFW subdirectory
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(dep/glfw)
# Add GLAD subdirectory
add_subdirectory(dep/glad)
# Add ImGui subdirectory
add_subdirectory(dep/imgui)

# Make project Pekan compile following source files into a static library
add_library(Pekan STATIC
    src/Pekan/Logger/PekanLogger.h
    src/Pekan/Logger/PekanLogger.cpp
    src/Pekan/PekanEngine.h
    src/Pekan/PekanEngine.cpp
    src/Pekan/PekanApplication.h
    src/Pekan/PekanApplication.cpp
    src/Pekan/PekanScene.h
    src/Pekan/PekanGUIWindow.h
    src/Pekan/PekanGUIWindow.cpp
)

# Group Logger files under a virtual folder called "Logger"
SOURCE_GROUP("Source Files\\Logger" FILES src/Pekan/Logger/PekanLogger.cpp)
SOURCE_GROUP("Header Files\\Logger" FILES src/Pekan/Logger/PekanLogger.h)

# Set link libraries for project Pekan
target_link_libraries(Pekan PRIVATE imgui glad glfw opengl32.lib)

# Set include directories for Pekan
target_include_directories(Pekan PUBLIC src/Pekan)

# Set project Demo to be the startup project by default
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo)
