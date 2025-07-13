# Create project GUI
project(GUI)

# Add a static library GUI, compiling the following source files
add_library(GUI STATIC
    GUI.h
    GUI.cpp
)

# Set include directories for GUI
target_include_directories(GUI PUBLIC .)

# Set link libraries for GUI
target_link_libraries(GUI PUBLIC Core)
target_link_libraries(GUI PRIVATE imgui)
