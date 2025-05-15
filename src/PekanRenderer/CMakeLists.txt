# Create project PekanRenderer
project(PekanRenderer)

# Add a static library PekanRenderer, compiling the following source files
add_library(PekanRenderer STATIC
    PekanRenderer.h
    PekanRenderer.cpp
    RenderObject.h
    RenderObject.cpp
    RenderComponent.h
    RenderComponent.cpp
    VertexBuffer.h
    VertexBuffer.cpp
    IndexBuffer.h
    IndexBuffer.cpp
    VertexArray.h
    VertexArray.cpp
    Shader.h
    Shader.cpp
)

# Set include directories for PekanRenderer
target_include_directories(PekanRenderer PUBLIC .)

# Set link libraries for PekanRenderer
target_link_libraries(PekanRenderer PRIVATE PekanCore glad glm opengl32.lib)
