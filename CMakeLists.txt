cmake_minimum_required(VERSION 3.6...3.31)

option(WITH_DEMO_PROJECTS "When this option is enabled, demo projects are included together with Pekan." ON)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set avaiable configurations to Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Create project Pekan
project(Pekan)

if(WITH_DEMO_PROJECTS)
    # Add Demos subdirectories
    add_subdirectory(src/Demo00)
    add_subdirectory(src/Demo01)
endif()
# Add PekanRenderer subdirectory
add_subdirectory(src/PekanRenderer)

# Add GLFW subdirectory
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(dep/glfw)
# Add GLAD subdirectory
add_subdirectory(dep/glad)
# Add ImGui subdirectory
add_subdirectory(dep/imgui)
# Add glm subdirectory
add_subdirectory(dep/glm)

# Add all projects for external libraries into a folder called "dep"
set_target_properties(glfw PROPERTIES FOLDER "dep")
set_target_properties(glad PROPERTIES FOLDER "dep")
set_target_properties(imgui PROPERTIES FOLDER "dep")
set_target_properties(glm PROPERTIES FOLDER "dep")

if(WITH_DEMO_PROJECTS)
    # Add all demo projects into a folder called "Demos"
    set_target_properties(Demo00 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo01 PROPERTIES FOLDER "Demos")
endif()

# Add a static library PekanCore, compiling the following source files
add_library(PekanCore STATIC
    src/PekanCore/Logger/PekanLogger.h
    src/PekanCore/Logger/PekanLogger.cpp
    src/PekanCore/PekanEngine.h
    src/PekanCore/PekanEngine.cpp
    src/PekanCore/PekanApplication.h
    src/PekanCore/PekanApplication.cpp
    src/PekanCore/PekanScene.h
    src/PekanCore/IPekanGUIWindow.h
    src/PekanCore/IPekanGUIWindow.cpp
    src/PekanCore/Utils/PekanUtils.h
    src/PekanCore/Utils/PekanUtils.cpp
)

# Group Logger files under a virtual folder called "Logger"
SOURCE_GROUP("Source Files\\Logger" FILES src/PekanCore/Logger/PekanLogger.cpp)
SOURCE_GROUP("Header Files\\Logger" FILES src/PekanCore/Logger/PekanLogger.h)
# Group Utils files under a virtual folder called "Utils"
SOURCE_GROUP("Source Files\\Utils" FILES src/PekanCore/Utils/PekanUtils.cpp)
SOURCE_GROUP("Header Files\\Utils" FILES src/PekanCore/Utils/PekanUtils.h)

# Set link libraries for PekanCore
target_link_libraries(PekanCore PRIVATE
    imgui
    glfw
    glm
    glad # TODO: This will probably NOT be need once we create OpenGL wrappers in PekanRenderer
    opengl32.lib # TODO: This will probably NOT be need once we create OpenGL wrappers in PekanRenderer
)

# Set include directories for PekanCore
target_include_directories(PekanCore PUBLIC src/PekanCore)

if(WITH_DEMO_PROJECTS)
    # Set Demo01 to be the startup project by default
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo01)
endif()