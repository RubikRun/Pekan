#include "PekanApplication.h"
#include "Logger/PekanLogger.h"
#include "PekanEngine.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
    bool PekanApplication::init()
    {
        // Initialize Pekan
        if (!PekanEngine::init(this, m_isFullScreen, m_shouldHideCursor))
        {
            PK_LOG_ERROR("Engine failed to initialize.", "Pekan");
            return false;
        }

        if (!_init())
        {
            return false;
        }

        // Init all layers
        for (Layer* layer : m_layerStack)
        {
            if (layer)
            {
                if (!layer->init())
                {
                    PK_LOG_ERROR("Failed to initialize layer \"" << layer->getName() << "\"", "Pekan");
                }
            }
            // A null layer is not really a problem, but it's also not supposed to happen at all,
            // so we can just log a warning.
            else
            {
                PK_LOG_WARNING("Null layer found when initializing application.", "Pekan");
            }
        }

        return true;
    }

    void PekanApplication::run()
	{
        GLFWwindow* window = PekanEngine::getWindow();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Close window if escape key is pressed
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }

            // Handle window resizing
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // Update all layers
            for (Layer* layer : m_layerStack)
            {
                if (layer)
                {
                    layer->update();
                }
            }
            // Render all layers
            for (Layer* layer : m_layerStack)
            {
                if (layer)
                {
                    layer->render();
                }
            }

            glfwSwapBuffers(window);
        }
	}

    void PekanApplication::exit()
    {
        // Exit all layers
        for (Layer* layer : m_layerStack)
        {
            if (layer)
            {
                layer->exit();
            }
        }
        // Exit engine
        PekanEngine::exit();
    }

    // Sends an event of a given type to layers of the layer stack,
    // one by one, until a layer successfully handles the event.
    template<typename EventT>
    static void _dispatchEvent(EventT& event, LayerStack& layerStack, bool (Layer::*onEventFunc)(EventT&))
    {
        for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
        {
            Layer* layer = *it;
            if (layer && (layer->*onEventFunc)(event))
            {
                break;
            }
        }
    }

    void PekanApplication::handleKeyEvent(int key, int scancode, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, false);
                _dispatchEvent(event, m_layerStack, &Layer::onKeyPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                _dispatchEvent(event, m_layerStack, &Layer::onKeyReleased);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, true);
                _dispatchEvent(event, m_layerStack, &Layer::onKeyPressed);
                break;
            }
        }
    }

    void PekanApplication::handleMouseMovedEvent(double xPos, double yPos)
    {
        MouseMovedEvent event = { float(xPos), float(yPos) };
        _dispatchEvent(event, m_layerStack, &Layer::onMouseMoved);
    }

    void PekanApplication::handleMouseScrolledEvent(double xOffset, double yOffset)
    {
        MouseScrolledEvent event = { float(xOffset), float(yOffset) };
        _dispatchEvent(event, m_layerStack, &Layer::onMouseScrolled);
    }

    void PekanApplication::handleMouseButtonEvent(int button, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                _dispatchEvent(event, m_layerStack, &Layer::onMouseButtonPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                _dispatchEvent(event, m_layerStack, &Layer::onMouseButtonReleased);
                break;
            }
        }
    }

    void PekanApplication::handleWindowResizedEvent(int width, int height)
    {
        WindowResizedEvent event(width, height);
        _dispatchEvent(event, m_layerStack, &Layer::onWindowResized);
    }

    void PekanApplication::handleWindowClosedEvent()
    {
        WindowClosedEvent event;
        _dispatchEvent(event, m_layerStack, &Layer::onWindowClosed);
    }

} // namespace Pekan