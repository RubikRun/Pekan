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
	EntityPropertiesGUIWindow.h
	EntityPropertiesGUIWindow.cpp
	ComponentWidgetsTypes.h
	ComponentWidgetsTypes.cpp
)

# Set link libraries for Editor
target_link_libraries(Editor PRIVATE
	Core
	Renderer2D
	GUI
)

# Set Editor's working directory to be Editor's source directory
set_target_properties(Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
