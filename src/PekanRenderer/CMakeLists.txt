# Create project PekanRenderer
project(PekanRenderer)

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
    2D/Shapes/CircleShape.cpp
)
SOURCE_GROUP("Header Files\\2D\\Shapes" FILES
    2D/Shapes/Shape.h
    2D/Shapes/TriangleShape.h
    2D/Shapes/RectangleShape.h
    2D/Shapes/CircleShape.h
)

# Set include directories for PekanRenderer
target_include_directories(PekanRenderer PUBLIC . RenderComponents 2D/Shapes)

# Set link libraries for PekanRenderer
target_link_libraries(PekanRenderer PRIVATE PekanCore glad glm opengl32.lib)

# Set PEKAN_RENDERER_ROOT_DIR macro to be the path to current source directory
target_compile_definitions(PekanRenderer PRIVATE PEKAN_RENDERER_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
