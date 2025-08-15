# Create project Graphics
project(Graphics)

# Add a static library Graphics, compiling the following source files
add_library(Graphics STATIC
    GraphicsSystem.h
    GraphicsSystem.cpp
    GLCall.h
    GLCall.cpp
    RenderState.h
    RenderState.cpp
    RenderCommands.h
    RenderCommands.cpp
    RenderObject.h
    RenderObject.cpp
    RenderComponents/VertexBuffer.h
    RenderComponents/VertexBuffer.cpp
    RenderComponents/IndexBuffer.h
    RenderComponents/IndexBuffer.cpp
    RenderComponents/VertexArray.h
    RenderComponents/VertexArray.cpp
    RenderComponents/Shader.h
    RenderComponents/Shader.cpp
    RenderComponents/Texture1D.h
    RenderComponents/Texture1D.cpp
    RenderComponents/Texture2D.h
    RenderComponents/Texture2D.cpp
    Image.h
    Image.cpp
    ShaderPreprocessor.h
    ShaderPreprocessor.cpp
)

# Group RenderComponents files under a virtual folder called "RenderComponents"
SOURCE_GROUP("Source Files\\RenderComponents" FILES
    RenderComponents/VertexBuffer.cpp
    RenderComponents/IndexBuffer.cpp
    RenderComponents/VertexArray.cpp
    RenderComponents/Shader.cpp
    RenderComponents/Texture1D.cpp
    RenderComponents/Texture2D.cpp
)
SOURCE_GROUP("Header Files\\RenderComponents" FILES
    RenderComponents/VertexBuffer.h
    RenderComponents/IndexBuffer.h
    RenderComponents/VertexArray.h
    RenderComponents/Shader.h
    RenderComponents/Texture1D.h
    RenderComponents/Texture2D.h
)

# Set include directories for Graphics
target_include_directories(Graphics PUBLIC . RenderComponents)

# Set link libraries for Graphics
target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE glad glfw opengl32.lib)
