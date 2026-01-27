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
    Core
    Graphics
    GUI
)

# Set Demo01's working directory to be Demo01's source directory
# so that we can write filepaths of resources relative to Demo01's source directory.
set_target_properties(Demo01 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Demo01>/resources
    COMMENT "Copying Demo01 resources to build directory"
)
