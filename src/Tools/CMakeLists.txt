# Create project Tools
project(Tools)

# Add a static library Tools, compiling the following source files
add_library(Tools STATIC
    PekanTools.h
    PekanTools.cpp
)

# Set include directories for Tools
target_include_directories(Tools PUBLIC .)

# Set link libraries for Tools
target_link_libraries(Tools PRIVATE Core Renderer2D)
