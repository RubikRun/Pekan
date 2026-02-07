# Create project Renderer2D
project(Renderer2D)

option(PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING "Enable orientation checking for 2D shapes" OFF)

# Add a static library Renderer2D, compiling the following source files
add_library(Renderer2D STATIC
	Renderer2DSubsystem.h
	Renderer2DSubsystem.cpp
	RenderSystem2D.h
	RenderSystem2D.cpp
	Utils2D.h
	Utils2D.cpp
	CameraComponent2D.h
	CameraComponent2D.cpp
	CameraSystem2D.h
	CameraSystem2D.cpp
	CameraController2D.h
	CameraController2D.cpp
	LineComponent.h
	LineSystem.h
	LineSystem.cpp
	TransformComponent2D.h
	TransformComponent2D.cpp
	TransformSystem2D.h
	TransformSystem2D.cpp
	Scene2D.h
	Scene2D.cpp
	VerticesAttributeView.h
	Shapes/RectangleGeometryComponent.h
	Shapes/RectangleGeometrySystem.h
	Shapes/RectangleGeometrySystem.cpp
	Shapes/LineGeometryComponent.h
	Shapes/LineGeometrySystem.h
	Shapes/LineGeometrySystem.cpp
	Shapes/CircleGeometryComponent.h
	Shapes/CircleGeometrySystem.h
	Shapes/CircleGeometrySystem.cpp
	Shapes/TriangleGeometryComponent.h
	Shapes/TriangleGeometrySystem.h
	Shapes/TriangleGeometrySystem.cpp
	Shapes/PolygonGeometryComponent.h
	Shapes/PolygonGeometrySystem.h
	Shapes/PolygonGeometrySystem.cpp
	Sprite/SpriteComponent.h
	Sprite/SpriteSystem.h
	Sprite/SpriteSystem.cpp
	Sprite/SpriteVertex.h
	Materials/SolidColorMaterialComponent.h
	Materials/SolidColorMaterialComponent.cpp
)

# Group Shapes files under a virtual folder called "Shapes"
SOURCE_GROUP("Source Files\\Shapes" FILES
	Shapes/RectangleGeometrySystem.cpp
	Shapes/LineGeometrySystem.cpp
	Shapes/CircleGeometrySystem.cpp
	Shapes/TriangleGeometrySystem.cpp
	Shapes/PolygonGeometrySystem.cpp
)
SOURCE_GROUP("Header Files\\Shapes" FILES
	Shapes/RectangleGeometryComponent.h
	Shapes/RectangleGeometrySystem.h
	Shapes/LineGeometryComponent.h
	Shapes/LineGeometrySystem.h
	Shapes/CircleGeometryComponent.h
	Shapes/CircleGeometrySystem.h
	Shapes/TriangleGeometryComponent.h
	Shapes/TriangleGeometrySystem.h
	Shapes/PolygonGeometryComponent.h
	Shapes/PolygonGeometrySystem.h
)
# Group Sprite files under a virtual folder called "Sprite"
SOURCE_GROUP("Source Files\\Sprite" FILES
	Sprite/SpriteSystem.cpp
)
SOURCE_GROUP("Header Files\\Sprite" FILES
	Sprite/SpriteComponent.h
	Sprite/SpriteSystem.h
	Sprite/SpriteVertex.h
)
# Group Materials files under a virtual folder called "Materials"
SOURCE_GROUP("Header Files\\Materials" FILES
	Materials/SolidColorMaterialComponent.h
)
SOURCE_GROUP("Source Files\\Materials" FILES
	Materials/SolidColorMaterialComponent.cpp
)

# Set include directories for Renderer2D
target_include_directories(Renderer2D PUBLIC . Shapes Sprite Materials)

# Set link libraries for Renderer2D
target_link_libraries(Renderer2D PUBLIC Graphics)

target_compile_definitions(Renderer2D PRIVATE
	# Set PEKAN_RENDERER2D_ROOT_DIR definition to be the path to current source directory
	PEKAN_RENDERER2D_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_definitions(Renderer2D PUBLIC
	# Set PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING definition to be 0 or 1 depending on the on/off state of the option
	PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING=$<IF:$<BOOL:${PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING}>,1,0>
)
