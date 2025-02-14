# Create project GLAD
project(glad)

# Make project GLAD compile following source file into a static library
add_library(glad STATIC src/glad.c)
# Add project GLAD under a folder called "dep"
set_target_properties(glad PROPERTIES FOLDER "dep")

# Add include directories for GLAD
target_include_directories(glad PUBLIC include)
