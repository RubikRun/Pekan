# Create project Demo00
project(Demo00)

# Add an executable Demo00, compiling the following source files
add_executable(Demo00
    main.cpp
    Demo00_Application.h
    Demo00_Application.cpp
    Demo00_Scene.h
    Demo00_Scene.cpp
    Demo00_GUIWindow.h
    Demo00_GUIWindow.cpp
)

# Set link libraries for Demo00
target_link_libraries(Demo00 PRIVATE
    PekanCore
    PekanRenderer
    glm
    imgui
)

# Set Demo00's working directory to be Demo00's source directory
# so that we can write filepaths of resources relative to Demo00's source directory.
set_target_properties(Demo00 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
