# Create project Renderer
project(Renderer)

option(PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING "Disable orientation checking for 2D shapes" OFF)

# Add a static library Renderer, compiling the following source files
add_library(Renderer STATIC
    RenderObject.h
    RenderObject.cpp
    2D/Renderer2D.h
    2D/Renderer2D.cpp
    2D/Camera2D.h
    2D/Camera2D.cpp
    2D/Line.h
    2D/Line.cpp
    2D/Shapes/Shape.h
    2D/Shapes/Shape.cpp
    2D/Shapes/TriangleShape.h
    2D/Shapes/TriangleShape.cpp
    2D/Shapes/RectangleShape.h
    2D/Shapes/RectangleShape.cpp
    2D/Shapes/CircleShape.h
    2D/Shapes/CircleShape.cpp
    2D/Shapes/CircleShapeStatic.h
    2D/Shapes/CircleShapeStatic_impl.h
    2D/Shapes/PolygonShape.h
    2D/Shapes/PolygonShape.cpp
    2D/Shapes/LineShape.h
    2D/Shapes/LineShape.cpp
)

# Group 2D files under a virtual folder called "2D"
SOURCE_GROUP("Source Files\\2D" FILES
    2D/Renderer2D.cpp
    2D/Camera2D.cpp
    2D/Line.cpp
)
SOURCE_GROUP("Header Files\\2D" FILES
    2D/Renderer2D.h
    2D/Camera2D.h
    2D/Line.h
)
# Group 2D Shapes files under a virtual folder called "2D/Shapes"
SOURCE_GROUP("Source Files\\2D\\Shapes" FILES
    2D/Shapes/Shape.cpp
    2D/Shapes/TriangleShape.cpp
    2D/Shapes/RectangleShape.cpp
    2D/Shapes/LineShape.cpp
    2D/Shapes/CircleShape.cpp
    2D/Shapes/PolygonShape.cpp
)
SOURCE_GROUP("Header Files\\2D\\Shapes" FILES
    2D/Shapes/Shape.h
    2D/Shapes/TriangleShape.h
    2D/Shapes/RectangleShape.h
    2D/Shapes/LineShape.h
    2D/Shapes/CircleShape.h
    2D/Shapes/CircleShapeStatic.h
    2D/Shapes/CircleShapeStatic_impl.h
    2D/Shapes/PolygonShape.h
)

# Set include directories for Renderer
target_include_directories(Renderer PUBLIC . 2D 2D/Shapes)

# Set link libraries for Renderer
target_link_libraries(Renderer PUBLIC Graphics)

target_compile_definitions(Renderer PRIVATE
    # Set PEKAN_RENDERER_ROOT_DIR definition to be the path to current source directory
    PEKAN_RENDERER_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    # Set PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING definition to be 0 or 1 depending on the on/off state of the option
    PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING=$<IF:$<BOOL:${PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING}>,1,0>
)
