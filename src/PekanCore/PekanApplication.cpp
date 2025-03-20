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

        if (m_scene != nullptr && !m_scene->init())
        {
            return false;
        }
        if (m_guiWindow != nullptr && !m_guiWindow->init())
        {
            return false;
        }

        return true;
    }

    void PekanApplication::run()
	{
        GLFWwindow* window = PekanEngine::getWindow();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Handle window resizing
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // Update and render scene
            if (m_scene != nullptr)
            {
                m_scene->update();
                m_scene->render();
            }
            // Render GUI window
            if (m_guiWindow != nullptr)
            {
                m_guiWindow->render();
            }

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