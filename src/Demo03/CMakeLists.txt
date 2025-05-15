# Create project Demo03
project(Demo03)

# Add an executable Demo03, compiling the following source files
add_executable(Demo03
    main.cpp
    Demo03_Application.h
    Demo03_Application.cpp
    Demo03_Scene.h
    Demo03_Scene.cpp
    Snake.h
    Snake.cpp
    Apple.h
    Apple.cpp
)

# Set link libraries for Demo03
target_link_libraries(Demo03 PRIVATE
    PekanCore
    PekanRenderer
    glm
    imgui
)

# Set Demo03's working directory to be Demo03's source directory
# so that we can write filepaths of resources relative to Demo03's source directory.
set_target_properties(Demo03 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
