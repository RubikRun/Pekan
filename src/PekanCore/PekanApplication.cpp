#include "PekanApplication.h"
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
            PK_LOG_ERROR("Engine failed to initialize.", "Pekan");
            return false;
        }

        if (!_init())
        {
            return false;
        }

        if (m_scene == nullptr)
        {
            PK_LOG_ERROR("Trying to initialize application but scene is NULL.", "Pekan");
            return false;
        }
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERROR("Trying to initialize application but GUI window is NULL.", "Pekan");
            return false;
        }

        if (!m_scene->init())
        {
            return false;
        }
        if (!m_guiWindow->init())
        {
            return false;
        }

        return true;
    }

    void PekanApplication::run()
	{
        if (m_scene == nullptr)
        {
            PK_LOG_ERROR("Trying to run application but scene is NULL.", "Pekan");
            return;
        }
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERROR("Trying to run application but GUI window is NULL.", "Pekan");
            return;
        }

        GLFWwindow* window = PekanEngine::getWindow();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Handle window resizing
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // Update scene
            m_scene->update();

            // Render scene and GUI
            m_scene->render();
            m_guiWindow->render();

            glfwSwapBuffers(window);
        }
	}

    void PekanApplication::exit()
    {
        if (m_scene != nullptr)
        {
            m_scene->exit();
        }
        PekanEngine::exit();
    }

} // namespace Pekan