#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
#include "Demo/DemoScene.h"
using Pekan::DemoScene;
using Pekan::PekanScene;
#include "Demo/DemoGUIWindow.h"
using Pekan::DemoGUIWindow;
using Pekan::PekanGUIWindow;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void)
{
    PekanEngine engine;
    engine.init();

    GLFWwindow* window = engine.getWindow();

    DemoGUIWindow demoGui;
    demoGui.init(window);

    DemoScene demoScene;
    demoScene.init(engine.getWindowWidth(), engine.getWindowHeight());
    demoScene.attachGUIWindow(&demoGui);

    // TEMP: Code below will be part of PekanEngine.
    // It should be a generic way to run the main loop, having just
    // - a pointer to window
    // - a pointer to PekanScene base class
    // - a pointer to PekanGUIWindow base class
    // and not caring of the specific scene and specific GUI window.
    // That's why I'm referencing the demo scene and the demo GUI window
    // as pointers of their base classes here,
    // and notice that the specific demoGui and demoScene are not used below.
    PekanGUIWindow* gui = &demoGui;
    PekanScene* scene = &demoScene;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        // Handle window resizing
        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Update scene
        scene->update();

        // Render scene and GUI
        scene->render();
        gui->render();

        glfwSwapBuffers(window);
    }

    scene->exit();
    engine.exit();

    return 0;
}