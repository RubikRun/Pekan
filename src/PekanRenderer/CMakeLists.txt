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
)

# Group Time files under a virtual folder called "Time"
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

# Set include directories for PekanRenderer
target_include_directories(PekanRenderer PUBLIC . RenderComponents)

# Set link libraries for PekanRenderer
target_link_libraries(PekanRenderer PRIVATE PekanCore glad glm opengl32.lib)
