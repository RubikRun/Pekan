# Create project Demo
project(Demo)

# Add an executable Demo, compiling the following source files
add_executable(Demo
    main.cpp
    DemoApplication.h
    DemoApplication.cpp
    DemoScene.h
    DemoScene.cpp
    DemoGUIWindow.h
    DemoGUIWindow.cpp
)

# Set link libraries for Demo
target_link_libraries(Demo PRIVATE
    PekanCore
    PekanRenderer
    imgui # TODO: This should be removed once Pekan provides ImGui wrappers
    glad # TODO: This should be removed once Pekan provides OpenGL wrappers
)
