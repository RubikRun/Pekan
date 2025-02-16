# Create project PekanRenderer
project(PekanRenderer)

# Add a static library PekanRenderer, compiling the following source files
add_library(PekanRenderer STATIC
    VertexBuffer.h
    VertexBuffer.cpp
)

# Set include directories for PekanRenderer
target_include_directories(PekanRenderer PUBLIC .)

# Set link libraries for PekanRenderer
target_link_libraries(PekanRenderer PRIVATE PekanCore glad opengl32.lib)
