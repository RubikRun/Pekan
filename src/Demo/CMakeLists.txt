# Create project Demo
project(Demo)

# Add an executable Demo, compiling the following source files
add_executable(Demo
    main.cpp
    DemoApplication.h
    DemoApplication.cpp
    DemoScene.h
    DemoScene.cpp
    DemoGUIWindow.h
    DemoGUIWindow.cpp
)

# Set link libraries for Demo
target_link_libraries(Demo PRIVATE
    PekanCore
    PekanRenderer
    glm
    imgui # TODO: This should be removed once Pekan provides ImGui wrappers
)

# Set Demo's working directory to be Demo's source directory
# so that we can write filepaths of resources relative to Demo's source directory.
set_target_properties(Demo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
