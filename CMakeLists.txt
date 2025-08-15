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
    add_subdirectory(demos/Demo00)
    add_subdirectory(demos/Demo01)
    add_subdirectory(demos/Demo02)
    add_subdirectory(demos/Demo03)
    add_subdirectory(demos/Demo04)
    add_subdirectory(demos/Demo05)
    add_subdirectory(demos/Demo06)
    add_subdirectory(demos/Demo07)
    add_subdirectory(demos/Demo08)
endif()
# Add Graphics subdirectory
add_subdirectory(src/Graphics)
# Add Renderer2D subdirectory
add_subdirectory(src/Renderer2D)
# Add GUI subdirectory
add_subdirectory(src/GUI)
# Add Tools subdirectory
add_subdirectory(src/Tools)

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
    set_target_properties(Demo06 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo07 PROPERTIES FOLDER "Demos")
    set_target_properties(Demo08 PROPERTIES FOLDER "Demos")
endif()

# Add a static library Core, compiling the following source files
add_library(Core STATIC
    src/Core/PekanEngine.h
    src/Core/PekanEngine.cpp
    src/Core/PekanApplication.h
    src/Core/PekanApplication.cpp
    src/Core/Window.h
    src/Core/Window.cpp
    src/Core/Layer.h
    src/Core/LayerStack.h
    src/Core/LayerStack.cpp
    src/Core/ISubsystem.h
    src/Core/SubsystemManager.h
    src/Core/SubsystemManager.cpp
    src/Core/Logger/PekanLogger.h
    src/Core/Logger/PekanLogger.cpp
    src/Core/Utils/PekanUtils.h
    src/Core/Utils/PekanUtils.cpp
    src/Core/Utils/FileUtils.h
    src/Core/Utils/FileUtils.cpp
    src/Core/Utils/MathUtils.h
    src/Core/Utils/MathUtils.cpp
    src/Core/Utils/stb.cpp
    src/Core/Events/Event.h
    src/Core/Events/WindowEvents.h
    src/Core/Events/KeyEvents.h
    src/Core/Events/KeyEvents_Enums.h
    src/Core/Events/MouseEvents_Enums.h
    src/Core/Events/MouseEvents.h
    src/Core/Events/EventListener.h
    src/Core/Time/FpsLimiter.h
    src/Core/Time/FpsLimiter.cpp
    src/Core/Time/DeltaTimer.h
    src/Core/Time/DeltaTimer.cpp
)

# Group Logger files under a virtual folder called "Logger"
SOURCE_GROUP("Source Files\\Logger" FILES src/Core/Logger/PekanLogger.cpp)
SOURCE_GROUP("Header Files\\Logger" FILES src/Core/Logger/PekanLogger.h)
# Group Utils files under a virtual folder called "Utils"
SOURCE_GROUP("Source Files\\Utils" FILES src/Core/Utils/PekanUtils.cpp src/Core/Utils/FileUtils.cpp src/Core/Utils/MathUtils.cpp src/Core/Utils/stb.cpp)
SOURCE_GROUP("Header Files\\Utils" FILES src/Core/Utils/PekanUtils.h src/Core/Utils/FileUtils.h src/Core/Utils/MathUtils.h)
# Group Events files under a virtual folder called "Events"
SOURCE_GROUP("Header Files\\Events" FILES
    src/Core/Events/Event.h
    src/Core/Events/WindowEvents.h
    src/Core/Events/KeyEvents.h
    src/Core/Events/KeyEvents_Enums.h
    src/Core/Events/MouseEvents.h
    src/Core/Events/MouseEvents_Enums.h
    src/Core/Events/EventListener.h
)
# Group Time files under a virtual folder called "Time"
SOURCE_GROUP("Source Files\\Time" FILES src/Core/Time/FpsLimiter.cpp src/Core/Time/DeltaTimer.cpp)
SOURCE_GROUP("Header Files\\Time" FILES src/Core/Time/FpsLimiter.h src/Core/Time/DeltaTimer.h)

# Set link libraries for Core
target_link_libraries(Core PRIVATE glfw)
target_link_libraries(Core PUBLIC glm)

# Set include directories for Core
target_include_directories(Core PUBLIC src/Core src/Core/Logger)
target_include_directories(Core PRIVATE dep)

if(WITH_DEMO_PROJECTS)
    # Set Demo08 to be the startup project by default
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo08)
endif()

# Set PEKAN_ROOT_DIR macro to be the path to current source directory
target_compile_definitions(Core PRIVATE PEKAN_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
