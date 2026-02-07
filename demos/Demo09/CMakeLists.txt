# Create project Demo09
project(Demo09)

# Add an executable Demo09, compiling the following source files
add_executable(Demo09
	main.cpp
	Demo09_Application.h
	Demo09_Application.cpp
	Demo09_Scene.h
	Demo09_Scene.cpp
	Demo09_GUIWindow.h
	Demo09_GUIWindow.cpp
)

# Set link libraries for Demo09
target_link_libraries(Demo09 PRIVATE
	Core
	Renderer2D
	GUI
)

# Set Demo09's working directory to be Demo09's source directory
set_target_properties(Demo09 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory so the executable can find them when run directly
add_custom_command(TARGET Demo09 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:Demo09>/resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/Shaders
		$<TARGET_FILE_DIR:Demo09>/Shaders
	COMMENT "Copying Demo09 resources and Shaders to build directory"
)
