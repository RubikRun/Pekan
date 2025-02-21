# Create project ImGui
project(imgui)

# Add a static library imgui, compiling the following source files
add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.cpp
)

# Set include directories for ImGui
target_include_directories(imgui PUBLIC . backends)
# Set link libraries for ImGui
target_link_libraries(imgui PUBLIC glfw)
