# Create project Demo08
project(Demo08)

# Add an executable Demo08, compiling the following source files
add_executable(Demo08
    main.cpp
    Demo08_Application.h
    Demo08_Application.cpp
    Demo08_Scene.h
    Demo08_Scene.cpp
    Demo08_GUIWindow.h
    Demo08_GUIWindow.cpp
)

# Set link libraries for Demo08
target_link_libraries(Demo08 PRIVATE
    Core
    Renderer2D
    GUI
)

# Set Demo08's working directory to be Demo08's source directory
# so that we can write filepaths of resources relative to Demo08's source directory.
set_target_properties(Demo08 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo08 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Demo08>/resources
    COMMENT "Copying Demo08 resources to build directory"
)
