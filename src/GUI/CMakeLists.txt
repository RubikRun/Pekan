# Create project GUI
project(GUI)

# Add a static library GUI, compiling the following source files
add_library(GUI STATIC
    GUISystem.h
    GUISystem.cpp
    GUIWindow.h
    GUIWindow.cpp
    Widgets/Widget.h
    Widgets/Widget.cpp
    Widgets/TextWidget.h
    Widgets/TextWidget.cpp
    Widgets/ColorEdit3Widget.h
    Widgets/ColorEdit3Widget.cpp
    Widgets/DragFloat2Widget.h
    Widgets/DragFloat2Widget.cpp
    Widgets/CheckboxWidget.h
    Widgets/CheckboxWidget.cpp
    Widgets/ButtonWidget.h
    Widgets/ButtonWidget.cpp
    Widgets/SliderIntWidget.h
    Widgets/SliderIntWidget.cpp
    Widgets/ColorEdit4Widget.h
    Widgets/ColorEdit4Widget.cpp
    Widgets/SeparatorWidget.h
    Widgets/SeparatorWidget.cpp
    Widgets/SliderFloatWidget.h
    Widgets/SliderFloatWidget.cpp
    Widgets/SliderFloat2Widget.h
    Widgets/SliderFloat2Widget.cpp
    Widgets/ComboBoxWidget.h
    Widgets/ComboBoxWidget.cpp
    Widgets/FPSDisplayWidget.h
    Widgets/FPSDisplayWidget.cpp
    Widgets/NewLineWidget.h
    Widgets/NewLineWidget.cpp
)

# Group Widgets files under a virtual folder called "Widgets"
SOURCE_GROUP("Source Files\\Widgets" FILES
    Widgets/TextWidget.cpp
    Widgets/ColorEdit3Widget.cpp
    Widgets/DragFloat2Widget.cpp
    Widgets/CheckboxWidget.cpp
    Widgets/ButtonWidget.cpp
    Widgets/SliderIntWidget.cpp
    Widgets/ColorEdit4Widget.cpp
    Widgets/Widget.cpp
    Widgets/SeparatorWidget.cpp
    Widgets/SliderFloatWidget.cpp
    Widgets/SliderFloat2Widget.cpp
    Widgets/ComboBoxWidget.cpp
    Widgets/FPSDisplayWidget.cpp
    Widgets/NewLineWidget.cpp
)
SOURCE_GROUP("Header Files\\Widgets" FILES
    Widgets/TextWidget.h
    Widgets/ColorEdit3Widget.h
    Widgets/DragFloat2Widget.h
    Widgets/CheckboxWidget.h
    Widgets/ButtonWidget.h
    Widgets/SliderIntWidget.h
    Widgets/ColorEdit4Widget.h
    Widgets/Widget.h
    Widgets/SeparatorWidget.h
    Widgets/SliderFloatWidget.h
    Widgets/SliderFloat2Widget.h
    Widgets/ComboBoxWidget.h
    Widgets/FPSDisplayWidget.h
    Widgets/NewLineWidget.h
)

# Set include directories for GUI
target_include_directories(GUI PUBLIC . Widgets)

# Set link libraries for GUI
target_link_libraries(GUI PUBLIC Core)
target_link_libraries(GUI PRIVATE imgui)
