# Create project Demo10
project(Demo10)

# Add an executable Demo10, compiling the following source files
add_executable(Demo10
    main.cpp
    Demo10_Application.h
    Demo10_Application.cpp
    Demo10_Scene.h
    Demo10_Scene.cpp
    Demo10_GUIWindow.h
    Demo10_GUIWindow.cpp
    Player.h
    Player.cpp
)

# Set link libraries for Demo10
target_link_libraries(Demo10 PRIVATE
    Core
    Renderer2D
    GUI
)

# Set Demo10's working directory to be Demo10's source directory
set_target_properties(Demo10 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy assets to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo10 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Demo10>/assets
    COMMENT "Copying Demo10 assets to build directory"
)
