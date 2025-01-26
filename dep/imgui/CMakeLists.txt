# Create project ImGui
project(imgui)

# Make project ImGui compile following source files into a library
add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.cpp
)
# Add project ImGui under a folder called "dep"
set_target_properties(imgui PROPERTIES FOLDER "dep")

# Set include directories for project ImGui
target_include_directories(imgui PUBLIC . backends)
# Set link libraries for project ImGui
target_link_libraries(imgui PUBLIC glfw)
