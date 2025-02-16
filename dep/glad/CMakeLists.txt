# Create project GLAD
project(glad)

# Add a static library glad, compiling the following source file
add_library(glad STATIC src/glad.c)
# Add GLAD under a folder called "dep"
set_target_properties(glad PROPERTIES FOLDER "dep")

# Add include directories for GLAD
target_include_directories(glad PUBLIC include)
