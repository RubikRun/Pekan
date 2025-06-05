# Create project PekanRenderer
project(PekanRenderer)

option(PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING "Disable orientation checking for 2D shapes" OFF)

# Add a static library PekanRenderer, compiling the following source files
add_library(PekanRenderer STATIC
    PekanRenderer.h
    PekanRenderer.cpp
    RenderObject.h
    RenderObject.cpp
    RenderComponents/RenderComponent.h
    RenderComponents/RenderComponent.cpp
    RenderComponents/VertexBuffer.h
    RenderComponents/VertexBuffer.cpp
    RenderComponents/IndexBuffer.h
    RenderComponents/IndexBuffer.cpp
    RenderComponents/VertexArray.h
    RenderComponents/VertexArray.cpp
    RenderComponents/Shader.h
    RenderComponents/Shader.cpp
    RenderComponents/Texture.h
    RenderComponents/Texture.cpp
    Image.h
    Image.cpp
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
    2D/Line.h
    2D/Line.cpp
)

# Group RenderComponents files under a virtual folder called "RenderComponents"
SOURCE_GROUP("Source Files\\RenderComponents" FILES
    RenderComponents/RenderComponent.cpp
    RenderComponents/VertexBuffer.cpp
    RenderComponents/IndexBuffer.cpp
    RenderComponents/VertexArray.cpp
    RenderComponents/Shader.cpp
    RenderComponents/Texture.cpp
)
SOURCE_GROUP("Header Files\\RenderComponents" FILES
    RenderComponents/RenderComponent.h
    RenderComponents/VertexBuffer.h
    RenderComponents/IndexBuffer.h
    RenderComponents/VertexArray.h
    RenderComponents/Shader.h
    RenderComponents/Texture.h
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
# Group 2D files under a virtual folder called "2D"
SOURCE_GROUP("Source Files\\2D" FILES
    2D/Line.cpp
)
SOURCE_GROUP("Header Files\\2D" FILES
    2D/Line.h
)

# Set include directories for PekanRenderer
target_include_directories(PekanRenderer PUBLIC . RenderComponents 2D/Shapes)

# Set link libraries for PekanRenderer
target_link_libraries(PekanRenderer PRIVATE PekanCore glad glm opengl32.lib)

target_compile_definitions(PekanRenderer PRIVATE
    # Set PEKAN_RENDERER_ROOT_DIR definition to be the path to current source directory
    PEKAN_RENDERER_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    # Set PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING definition to be 0 or 1 depending on the on/off state of the option
    PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING=$<IF:$<BOOL:${PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING}>,1,0>
)
