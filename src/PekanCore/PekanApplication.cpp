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

        this->m_window = PekanEngine::getWindow();
        this->m_width = PekanEngine::getWindowWidth();
        this->m_height = PekanEngine::getWindowHeight();

        if (!_init())
        {
            return false;
        }

        if (m_scene == nullptr)
        {
            PK_LOG_ERRORF("Trying to initialize application but scene is NULL.");
            return false;
        }
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERRORF("Trying to initialize application but GUI window is NULL.");
            return false;
        }

        if (!m_scene->init(this->m_width, this->m_height))
        {
            return false;
        }
        if (!m_guiWindow->init(this->m_window))
        {
            return false;
        }

        return true;
    }

    void PekanApplication::run()
	{
        if (m_window == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but window is NULL.");
            return;
        }
        if (m_scene == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but scene is NULL.");
            return;
        }
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERRORF("Trying to run application but GUI window is NULL.");
            return;
        }

        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();

            // Handle window resizing
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // Update scene
            m_scene->update();

            // Render scene and GUI
            m_scene->render();
            m_guiWindow->render();

            glfwSwapBuffers(m_window);
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