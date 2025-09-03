#include "Window.h"

#include "PekanLogger.h"
#include "PekanEngine.h"
#include "PekanApplication.h"

#include <GLFW/glfw3.h>

#define PK_OPENGL_VERSION_MAJOR 4
#define PK_OPENGL_VERSION_MINOR 3

namespace Pekan
{

    bool Window::create(const ApplicationProperties& applicationProperties)
    {
        m_properties = applicationProperties.windowProperties;

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
        // Set window hint for number of samples
        glfwWindowHint(GLFW_SAMPLES, applicationProperties.numberOfSamples);

        // Create a GLFW window
        if (m_properties.fullScreen)
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

            m_glfwWindow = glfwCreateWindow(mode->width, mode->height, m_properties.title.c_str(), primaryMonitor, nullptr);
        }
        else
        {
            m_glfwWindow = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), nullptr, nullptr);
        }
        if (m_glfwWindow == nullptr)
        {
            PK_LOG_ERROR("Failed to create a window with GLFW.", "Pekan");
            glfwTerminate();
            return false;
        }

        if (!m_properties.fullScreen)
        {
            // Set window's initial position
            glfwSetWindowPos(m_glfwWindow, m_properties.initialPosition.x, m_properties.initialPosition.y);
        }

        // Make the window's context current
        glfwMakeContextCurrent(m_glfwWindow);

        // Hide cursor if needed
        if (m_properties.hideCursor)
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

    bool Window::isKeyPressed(KeyCode key) const
    {
        return (glfwGetKey(m_glfwWindow, int(key)) == GLFW_PRESS || glfwGetKey(m_glfwWindow, int(key)) == GLFW_REPEAT);
    }

    bool Window::isKeyReleased(KeyCode key) const
    {
        return (glfwGetKey(m_glfwWindow, int(key)) == GLFW_RELEASE);
    }

    bool Window::isKeyRepeating(KeyCode key) const
    {
        return (glfwGetKey(m_glfwWindow, int(key)) == GLFW_REPEAT);
    }

    glm::vec2 Window::getMousePosition() const
    {
        double xMouse = 0.0;
        double yMouse = 0.0;
        glfwGetCursorPos(m_glfwWindow, &xMouse, &yMouse);
        return { float(xMouse), float(yMouse) };
    }

    bool Window::isMouseButtonPressed(MouseButton button) const
    {
        return glfwGetMouseButton(m_glfwWindow, int(button)) == GLFW_PRESS;
    }

    bool Window::isMouseButtonReleased(MouseButton button) const
    {
        return glfwGetMouseButton(m_glfwWindow, int(button)) == GLFW_RELEASE;
    }

    glm::ivec2 Window::getSize() const
    {
        int width, height;
        glfwGetWindowSize(m_glfwWindow, &width, &height);
        return { width, height };
    }

    int Window::getSizeSmaller() const
    {
        const glm::ivec2 size = getSize();
        if (size.x < size.y)
        {
            return size.x;
        }
        return size.y;
    }

    int Window::getSizeBigger() const
    {
        const glm::ivec2 size = getSize();
        if (size.x > size.y)
        {
            return size.x;
        }
        return size.y;
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
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleKeyEvent(KeyCode(key), scancode, action, mods);
        }
    }
    void Window::mouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
    {
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleMouseMovedEvent(xPos, yPos);
        }
    }
    void Window::mouseScrolledCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleMouseScrolledEvent(xOffset, yOffset);
        }
    }
    void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleMouseButtonEvent(MouseButton(button), action, mods);
        }
    }
    void Window::windowResizedCallback(GLFWwindow* window, int width, int height)
    {
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleWindowResizedEvent(width, height);
        }
    }
    void Window::windowClosedCallback(GLFWwindow* window)
    {
        PekanApplication* application = PekanEngine::getApplication();
        if (application != nullptr)
        {
            application->handleWindowClosedEvent();
        }
    }

} // namespace Pekan