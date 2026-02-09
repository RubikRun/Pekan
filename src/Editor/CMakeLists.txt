# Create project Editor
project(Editor)

# Add an executable Editor, compiling the following source files
add_executable(Editor
	main.cpp
	EditorApplication.h
	EditorApplication.cpp
	EditorScene.h
	EditorScene.cpp
	EntitiesGUIWindow.h
	EntitiesGUIWindow.cpp
)

# Set link libraries for Editor
target_link_libraries(Editor PRIVATE
	Core
	Renderer2D
	GUI
)

# Set Editor's working directory to be Editor's source directory
set_target_properties(Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy assets to build directory so the executable can find them when run directly
add_custom_command(TARGET Editor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:Editor>/assets
	COMMENT "Copying Editor assets to build directory"
)
