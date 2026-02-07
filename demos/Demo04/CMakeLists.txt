# Create project Demo04
project(Demo04)

# Add an executable Demo04, compiling the following source files
add_executable(Demo04
	main.cpp
	Demo04_Application.h
	Demo04_Application.cpp
	Demo04_Scene.h
	Demo04_Scene.cpp
	Demo04_GUIWindow.h
	Demo04_GUIWindow.cpp
)

# Set link libraries for Demo04
target_link_libraries(Demo04 PRIVATE
	Core
	Renderer2D
	GUI
)

# Set Demo04's working directory to be Demo04's source directory
# so that we can write filepaths of resources relative to Demo04's source directory.
set_target_properties(Demo04 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo04 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:Demo04>/resources
	COMMENT "Copying Demo04 resources to build directory"
)
