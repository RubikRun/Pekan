# Create project Demo01
project(Demo01)

# Add an executable Demo01, compiling the following source files
add_executable(Demo01
    main.cpp
    Demo01_Application.h
    Demo01_Application.cpp
    Demo01_Scene.h
    Demo01_Scene.cpp
    Demo01_GUIWindow.h
    Demo01_GUIWindow.cpp
)

# Set link libraries for Demo01
target_link_libraries(Demo01 PRIVATE
    PekanCore
    PekanRenderer
    glm
    imgui
)

# Set Demo01's working directory to be Demo01's source directory
# so that we can write filepaths of resources relative to Demo01's source directory.
set_target_properties(Demo01 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
