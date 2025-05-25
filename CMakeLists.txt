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
    add_subdirectory(src/Demo02)
    add_subdirectory(src/Demo03)
    add_subdirectory(src/Demo04)
    add_subdirectory(src/Demo05)
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
    set_target_properties(Demo02 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo03 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo04 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo05 PROPERTIES FOLDER "Demos")
endif()

# Add a static library PekanCore, compiling the following source files
add_library(PekanCore STATIC
    src/PekanCore/Logger/PekanLogger.h
    src/PekanCore/Logger/PekanLogger.cpp
    src/PekanCore/PekanEngine.h
    src/PekanCore/PekanEngine.cpp
    src/PekanCore/PekanApplication.h
    src/PekanCore/PekanApplication.cpp
    src/PekanCore/PekanGUIWindow.h
    src/PekanCore/PekanGUIWindow.cpp
    src/PekanCore/Window.h
    src/PekanCore/Window.cpp
    src/PekanCore/Layer.h
    src/PekanCore/Layer.cpp
    src/PekanCore/LayerStack.h
    src/PekanCore/LayerStack.cpp
    src/PekanCore/Utils/PekanUtils.h
    src/PekanCore/Utils/PekanUtils.cpp
    src/PekanCore/Utils/FileUtils.h
    src/PekanCore/Utils/FileUtils.cpp
    src/PekanCore/Utils/stb.cpp
    src/PekanCore/Events/Event.h
    src/PekanCore/Events/WindowEvent.h
    src/PekanCore/Events/KeyEvent.h
    src/PekanCore/Events/KeyEvent_Enums.h
    src/PekanCore/Events/MouseEvent_Enums.h
    src/PekanCore/Events/MouseEvent.h
    src/PekanCore/Time/FpsLimiter.h
    src/PekanCore/Time/FpsLimiter.cpp
    src/PekanCore/Time/DeltaTimer.h
    src/PekanCore/Time/DeltaTimer.cpp
)

# Group Logger files under a virtual folder called "Logger"
SOURCE_GROUP("Source Files\\Logger" FILES src/PekanCore/Logger/PekanLogger.cpp)
SOURCE_GROUP("Header Files\\Logger" FILES src/PekanCore/Logger/PekanLogger.h)
# Group Utils files under a virtual folder called "Utils"
SOURCE_GROUP("Source Files\\Utils" FILES src/PekanCore/Utils/PekanUtils.cpp src/PekanCore/Utils/FileUtils.cpp src/PekanCore/Utils/stb.cpp)
SOURCE_GROUP("Header Files\\Utils" FILES src/PekanCore/Utils/PekanUtils.h src/PekanCore/Utils/FileUtils.h)
# Group Events files under a virtual folder called "Events"
SOURCE_GROUP("Header Files\\Events" FILES
    src/PekanCore/Events/Event.h
    src/PekanCore/Events/WindowEvent.h
    src/PekanCore/Events/KeyEvent.h
    src/PekanCore/Events/KeyEvent_Enums.h
    src/PekanCore/Events/MouseEvent.h
    src/PekanCore/Events/MouseEvent_Enums.h
)
# Group Time files under a virtual folder called "Time"
SOURCE_GROUP("Source Files\\Time" FILES src/PekanCore/Time/FpsLimiter.cpp src/PekanCore/Time/DeltaTimer.cpp)
SOURCE_GROUP("Header Files\\Time" FILES src/PekanCore/Time/FpsLimiter.h src/PekanCore/Time/DeltaTimer.h)

# Set link libraries for PekanCore
target_link_libraries(PekanCore PRIVATE
    imgui
    glfw
    glm
    glad
    opengl32.lib
)

# Set include directories for PekanCore
target_include_directories(PekanCore PUBLIC src/PekanCore)
target_include_directories(PekanCore PRIVATE dep)

if(WITH_DEMO_PROJECTS)
    # Set Demo05 to be the startup project by default
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo05)
endif()

# Set PEKAN_ROOT_DIR macro to be the path to current source directory
target_compile_definitions(PekanCore PRIVATE PEKAN_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
