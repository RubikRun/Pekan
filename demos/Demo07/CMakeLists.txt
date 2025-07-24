# Create project Demo07
project(Demo07)

# Add an executable Demo07, compiling the following source files
add_executable(Demo07
    main.cpp
    Demo07_Application.h
    Demo07_Application.cpp
    Demo07_Scene.h
    Demo07_Scene.cpp
    Demo07_GUIWindow.h
    Demo07_GUIWindow.cpp
)

# Set link libraries for Demo07
target_link_libraries(Demo07 PRIVATE
    Core
    Renderer
    Tools
    GUI
)

# Set Demo07's working directory to be Demo07's source directory
# so that we can write filepaths of resources relative to Demo07's source directory.
set_target_properties(Demo07 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
