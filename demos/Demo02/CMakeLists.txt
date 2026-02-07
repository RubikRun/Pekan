# Create project Demo02
project(Demo02)

# Add an executable Demo02, compiling the following source files
add_executable(Demo02
	main.cpp
	Demo02_Application.h
	Demo02_Application.cpp
	Demo02_Scene.h
	Demo02_Scene.cpp
	Demo02_GUIWindow.h
	Demo02_GUIWindow.cpp
)

# Set link libraries for Demo02
target_link_libraries(Demo02 PRIVATE
	Core
	Graphics
	GUI
)

# Set Demo02's working directory to be Demo02's source directory
# so that we can write filepaths of resources relative to Demo02's source directory.
set_target_properties(Demo02 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo02 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:Demo02>/resources
	COMMENT "Copying Demo02 resources to build directory"
)
