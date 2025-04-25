#include "Window.h"

#include "Logger/PekanLogger.h"
#include "PekanEngine.h"
#include "PekanApplication.h"

#include <GLFW/glfw3.h>

#define PK_OPENGL_VERSION_MAJOR 4
#define PK_OPENGL_VERSION_MINOR 3

namespace Pekan
{

    bool Window::create(const WindowProperties& properties)
    {
        m_properties = properties;

        if (!glfwInit())
        {
            PK_LOG_ERROR("Failed to initialize GLFW.", "Pekan");
            return false;
        }

        // Set window hint for OpenGL version and OpenGL Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, PK_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, PK_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        // Create a GLFW window
        if (properties.fullScreen)
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

            m_glfwWindow = glfwCreateWindow(mode->width, mode->height, properties.title.c_str(), primaryMonitor, nullptr);
        }
        else
        {
            m_glfwWindow = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), nullptr, nullptr);
        }
        if (m_glfwWindow == nullptr)
        {
            PK_LOG_ERROR("Failed to create a window with GLFW.", "Pekan");
            glfwTerminate();
            return false;
        }
        // Make the window's context current
        glfwMakeContextCurrent(m_glfwWindow);

        // Hide cursor if needed
        if (properties.hideCursor)
        {
            glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        setEventCallbacks();

        return true;
    }

    void Window::destroy()
    {
        if (m_glfwWindow)
        {
            glfwDestroyWindow(m_glfwWindow);
            glfwTerminate();
            m_glfwWindow = nullptr;
        }
        else
        {
            PK_LOG_ERROR("Trying to destroy window but window is not yet created.", "Pekan");
        }
    }

    void Window::enableVSync()
    {
        glfwSwapInterval(1);
    }

    void Window::disableVSync()
    {
        glfwSwapInterval(0);
    }

    bool Window::shouldBeClosed() const
    {
        return glfwWindowShouldClose(m_glfwWindow);
    }

    void Window::setShouldBeClosed(bool enabled)
    {
        glfwSetWindowShouldClose(m_glfwWindow, enabled);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_glfwWindow);
    }

    bool Window::isMinimized() const
    {
        return glfwGetWindowAttrib(m_glfwWindow, GLFW_ICONIFIED) != 0;
    }

    bool Window::isKeyPressed(int key) const
    {
        return (glfwGetKey(m_glfwWindow, key) == GLFW_PRESS || glfwGetKey(m_glfwWindow, key) == GLFW_REPEAT);
    }

    bool Window::isKeyReleased(int key) const
    {
        return (glfwGetKey(m_glfwWindow, key) == GLFW_RELEASE);
    }

    bool Window::isKeyRepeating(int key) const
    {
        return (glfwGetKey(m_glfwWindow, key) == GLFW_REPEAT);
    }

    glm::vec2 Window::getMousePosition() const
    {
        double xMouse = 0.0;
        double yMouse = 0.0;
        glfwGetCursorPos(m_glfwWindow, &xMouse, &yMouse);
        return { float(xMouse), float(yMouse) };
    }

    bool Window::isMouseButtonPressed(bool leftOrRight) const
    {
        return glfwGetMouseButton(m_glfwWindow, leftOrRight ? 0 : 1) == GLFW_PRESS;
    }

    bool Window::isMouseButtonReleased(bool leftOrRight) const
    {
        return glfwGetMouseButton(m_glfwWindow, leftOrRight ? 0 : 1) == GLFW_RELEASE;
    }

    glm::ivec2 Window::getResolution() const
    {
        int width, height;
        glfwGetWindowSize(m_glfwWindow, &width, &height);
        return { width, height };
    }

    glm::ivec2 Window::getFrameBufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_glfwWindow, &width, &height);
        return { width, height };
    }

    void Window::setEventCallbacks()
    {
        glfwSetKeyCallback(m_glfwWindow, keyCallback);
        glfwSetCursorPosCallback(m_glfwWindow, mouseMovedCallback);
        glfwSetScrollCallback(m_glfwWindow, mouseScrolledCallback);
        glfwSetMouseButtonCallback(m_glfwWindow, mouseButtonCallback);
        glfwSetWindowSizeCallback(m_glfwWindow, windowResizedCallback);
        glfwSetWindowCloseCallback(m_glfwWindow, windowClosedCallback);
    }

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleKeyEvent(key, scancode, action, mods);
        }
    }
    void Window::mouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleMouseMovedEvent(xPos, yPos);
        }
    }
    void Window::mouseScrolledCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleMouseScrolledEvent(xOffset, yOffset);
        }
    }
    void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleMouseButtonEvent(button, action, mods);
        }
    }
    void Window::windowResizedCallback(GLFWwindow* window, int width, int height)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleWindowResizedEvent(width, height);
        }
    }
    void Window::windowClosedCallback(GLFWwindow* window)
    {
        if (PekanEngine::s_application)
        {
            PekanEngine::s_application->handleWindowClosedEvent();
        }
    }

} // namespace Pekan