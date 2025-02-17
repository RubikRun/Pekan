#include "PekanApplication.h"
#define PK_FILENAME "PekanApplication.cpp"
#include "Logger/PekanLogger.h"

#include "PekanEngine.h"
#include "PekanScene.h"
#include "PekanGUIWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
    bool PekanApplication::init()
    {
        if (!PekanEngine::init())
        {
            PK_LOG_ERRORF("Engine failed to initialize.");
            return false;
        }

        this->window = PekanEngine::getWindow();
        this->width = PekanEngine::getWindowWidth();
        this->height = PekanEngine::getWindowHeight();

        if (!_init())
        {
            return false;
        }

        if (scene == nullptr)
        {
            PK_LOG_ERRORF("Trying to initialize application but scene is NULL.");
            return false;
        }
        if (guiWindow == nullptr)
        {
            PK_LOG_ERRORF("Trying to initialize application but GUI window is NULL.");
            return false;
        }

        if (!scene->init(this->width, this->height))
        {
            return false;
        }
        if (!guiWindow->init(this->window))
        {
            return false;
        }

        return true;
    }

    void PekanApplication::run()
	{
        if (window == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but window is NULL.");
            return;
        }
        if (scene == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but scene is NULL.");
            return;
        }
        if (guiWindow == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but GUI window is NULL.");
            return;
        }

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
            guiWindow->render();

            glfwSwapBuffers(window);
        }
	}

    void PekanApplication::exit()
    {
        if (scene != nullptr)
        {
            scene->exit();
        }
        PekanEngine::exit();
    }

} // namespace Pekan