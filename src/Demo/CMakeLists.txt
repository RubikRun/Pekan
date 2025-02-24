# Create project Demo
project(Demo)

# Add an executable Demo, compiling the following source files
add_executable(Demo
    main.cpp
    Demo00_Application.h
    Demo00_Application.cpp
    Demo00_Scene.h
    Demo00_Scene.cpp
    Demo00_GUIWindow.h
    Demo00_GUIWindow.cpp
    Demo01_Application.h
    Demo01_Application.cpp
    Demo01_Scene.h
    Demo01_Scene.cpp
    Demo01_GUIWindow.h
    Demo01_GUIWindow.cpp
)

# Set link libraries for Demo
target_link_libraries(Demo PRIVATE
    PekanCore
    PekanRenderer
    glm
    glad
    imgui # TODO: This should be removed once Pekan provides ImGui wrappers
)

# Set Demo's working directory to be Demo's source directory
# so that we can write filepaths of resources relative to Demo's source directory.
set_target_properties(Demo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
