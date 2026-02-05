# Create project Graphics
project(Graphics)

# Add a static library Graphics, compiling the following source files
add_library(Graphics STATIC
    GraphicsSubsystem.h
    GraphicsSubsystem.cpp
    GLCall.h
    GLCall.cpp
    RenderState.h
    RenderState.cpp
    RenderCommands.h
    RenderCommands.cpp
    DrawObject.h
    DrawObject.cpp
    GpuResources/VertexBuffer.h
    GpuResources/VertexBuffer.cpp
    GpuResources/IndexBuffer.h
    GpuResources/IndexBuffer.cpp
    GpuResources/VertexArray.h
    GpuResources/VertexArray.cpp
    GpuResources/Shader.h
    GpuResources/Shader.cpp
    GpuResources/Texture1D.h
    GpuResources/Texture1D.cpp
    GpuResources/Texture2D.h
    GpuResources/Texture2D.cpp
    GpuResources/Texture2DMultisample.h
    GpuResources/Texture2DMultisample.cpp
    GpuResources/FrameBuffer.h
    GpuResources/FrameBuffer.cpp
    GpuResources/RenderBuffer.h
    GpuResources/RenderBuffer.cpp
    Image.h
    Image.cpp
    ShaderPreprocessor.h
    ShaderPreprocessor.cpp
    PostProcessor.h
    PostProcessor.cpp
)

# Group GpuResources files under a virtual folder called "GpuResources"
SOURCE_GROUP("Source Files\\GpuResources" FILES
    GpuResources/VertexBuffer.cpp
    GpuResources/IndexBuffer.cpp
    GpuResources/VertexArray.cpp
    GpuResources/Shader.cpp
    GpuResources/Texture1D.cpp
    GpuResources/Texture2D.cpp
    GpuResources/Texture2DMultisample.cpp
    GpuResources/FrameBuffer.cpp
    GpuResources/RenderBuffer.cpp
)
SOURCE_GROUP("Header Files\\GpuResources" FILES
    GpuResources/VertexBuffer.h
    GpuResources/IndexBuffer.h
    GpuResources/VertexArray.h
    GpuResources/Shader.h
    GpuResources/Texture1D.h
    GpuResources/Texture2D.h
    GpuResources/Texture2DMultisample.h
    GpuResources/FrameBuffer.h
    GpuResources/RenderBuffer.h
)

# Set include directories for Graphics
target_include_directories(Graphics PUBLIC . GpuResources)

# Set link libraries for Graphics
target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE glad glfw opengl32.lib)

target_compile_definitions(Graphics PRIVATE
    # Set PEKAN_GRAPHICS_ROOT_DIR definition to be the path to current source directory
    PEKAN_GRAPHICS_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
