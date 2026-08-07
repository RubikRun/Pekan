# Create project DemoIg00
project(DemoIg00)

# Add an executable DemoIg00, compiling the following source files
add_executable(DemoIg00
	main.cpp
	DemoIg00_Application.h
	DemoIg00_Application.cpp
	DemoIg00_Scene.h
	DemoIg00_Scene.cpp
	EntityVisualState.h
	IEntityRenderer.h
	DirectSpriteRenderer.h
	DirectSpriteRenderer.cpp
	SwimmingDotsRenderer.h
	SwimmingDotsRenderer.cpp
	FunctionGraphsRenderer.h
	FunctionGraphsRenderer.cpp
	ColorGridRenderer.h
	ColorGridRenderer.cpp
	Player.h
	Player.cpp
)

# Set link libraries for DemoIg00
target_link_libraries(DemoIg00 PRIVATE
	Core
	Renderer2D
)

# Set DemoIg00's working directory to be DemoIg00's source directory
# so that we can write filepaths of resources relative to DemoIg00's source directory.
set_target_properties(DemoIg00 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
