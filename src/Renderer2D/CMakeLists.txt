# Create project Renderer2D
project(Renderer2D)

option(PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING "Disable orientation checking for 2D shapes" OFF)

# Add a static library Renderer2D, compiling the following source files
add_library(Renderer2D STATIC
    RenderObject.h
    RenderObject.cpp
    Renderer2DSystem.h
    Renderer2DSystem.cpp
    Camera2D.h
    Camera2D.cpp
    Line.h
    Line.cpp
    Shapes/Shape.h
    Shapes/Shape.cpp
    Shapes/TriangleShape.h
    Shapes/TriangleShape.cpp
    Shapes/RectangleShape.h
    Shapes/RectangleShape.cpp
    Shapes/CircleShape.h
    Shapes/CircleShape.cpp
    Shapes/CircleShapeStatic.h
    Shapes/CircleShapeStatic_impl.h
    Shapes/PolygonShape.h
    Shapes/PolygonShape.cpp
    Shapes/LineShape.h
    Shapes/LineShape.cpp
    Shapes/ShapesBatch.h
    Shapes/ShapesBatch.cpp
)

# Group Shapes files under a virtual folder called "Shapes"
SOURCE_GROUP("Source Files\\Shapes" FILES
    Shapes/Shape.cpp
    Shapes/TriangleShape.cpp
    Shapes/RectangleShape.cpp
    Shapes/LineShape.cpp
    Shapes/CircleShape.cpp
    Shapes/PolygonShape.cpp
    Shapes/ShapesBatch.cpp
)
SOURCE_GROUP("Header Files\\Shapes" FILES
    Shapes/Shape.h
    Shapes/TriangleShape.h
    Shapes/RectangleShape.h
    Shapes/LineShape.h
    Shapes/CircleShape.h
    Shapes/CircleShapeStatic.h
    Shapes/CircleShapeStatic_impl.h
    Shapes/PolygonShape.h
    Shapes/ShapesBatch.h
)

# Set include directories for Renderer2D
target_include_directories(Renderer2D PUBLIC . Shapes)

# Set link libraries for Renderer2D
target_link_libraries(Renderer2D PUBLIC Graphics)

target_compile_definitions(Renderer2D PRIVATE
    # Set PEKAN_RENDERER2D_ROOT_DIR definition to be the path to current source directory
    PEKAN_RENDERER2D_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    # Set PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING definition to be 0 or 1 depending on the on/off state of the option
    PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING=$<IF:$<BOOL:${PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING}>,1,0>
)
