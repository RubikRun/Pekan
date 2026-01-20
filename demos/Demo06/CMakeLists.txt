# Create project Demo06
project(Demo06)

# Add an executable Demo06, compiling the following source files
add_executable(Demo06
    main.cpp
    Demo06_Application.h
    Demo06_Application.cpp
    Demo06_Scene.h
    Demo06_Scene.cpp
    Demo06_GUIWindow.h
    Demo06_GUIWindow.cpp
)

# Set link libraries for Demo06
target_link_libraries(Demo06 PRIVATE
    Core
    Renderer2D
    Tools
    GUI
)

# Set Demo06's working directory to be Demo06's source directory
# so that we can write filepaths of resources relative to Demo06's source directory.
set_target_properties(Demo06 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy Shaders to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo06 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Shaders
        $<TARGET_FILE_DIR:Demo06>/Shaders
    COMMENT "Copying Demo06 Shaders to build directory"
)
