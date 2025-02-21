# Create project GLAD
project(glad)

# Add a static library glad, compiling the following source file
add_library(glad STATIC src/glad.c)

# Add include directories for GLAD
target_include_directories(glad PUBLIC include)
