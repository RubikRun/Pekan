# Create project Demo05
project(Demo05)

# Add an executable Demo05, compiling the following source files
add_executable(Demo05
    main.cpp
    Demo05_Application.h
    Demo05_Application.cpp
    Demo05_Scene.h
    Demo05_Scene.cpp
    Demo05_GUIWindow.h
    Demo05_GUIWindow.cpp
)

# Set link libraries for Demo05
target_link_libraries(Demo05 PRIVATE
    PekanCore
    PekanRenderer
    glm
    imgui
)

# Set Demo05's working directory to be Demo05's source directory
# so that we can write filepaths of resources relative to Demo05's source directory.
set_target_properties(Demo05 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
