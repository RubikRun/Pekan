# Create project GUI
project(GUI)

# Add a static library GUI, compiling the following source files
add_library(GUI STATIC
    GUI.h
    GUI.cpp
    Widgets/TextWidget.h
    Widgets/TextWidget.cpp
    Widgets/ColorEdit3Widget.h
    Widgets/ColorEdit3Widget.cpp
    Widgets/DragFloat2Widget.h
    Widgets/DragFloat2Widget.cpp
)

# Group Widgets files under a virtual folder called "Widgets"
SOURCE_GROUP("Source Files\\Widgets" FILES
    Widgets/TextWidget.cpp
    Widgets/ColorEdit3Widget.cpp
    Widgets/DragFloat2Widget.cpp
)
SOURCE_GROUP("Header Files\\Widgets" FILES
    Widgets/TextWidget.h
    Widgets/ColorEdit3Widget.h
    Widgets/DragFloat2Widget.h
)

# Set include directories for GUI
target_include_directories(GUI PUBLIC . Widgets)

# Set link libraries for GUI
target_link_libraries(GUI PUBLIC Core)
target_link_libraries(GUI PRIVATE imgui)
