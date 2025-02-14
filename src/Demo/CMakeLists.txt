# Create project Demo
project(Demo)

# Make project Demo compile following source files into an executable
add_executable(Demo
    main.cpp
    DemoApplication.h
    DemoApplication.cpp
    DemoScene.h
    DemoScene.cpp
    DemoGUIWindow.h
    DemoGUIWindow.cpp
)

# Set link libraries for project Demo
target_link_libraries(Demo PRIVATE
    Pekan
    imgui # TODO: This should be removed once Pekan provides ImGui wrappers
    glad # TODO: This should be removed once Pekan provides OpenGL wrappers
)
