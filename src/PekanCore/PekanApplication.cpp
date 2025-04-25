#include "PekanApplication.h"
#include "Logger/PekanLogger.h"
#include "PekanEngine.h"
#include "FpsLimiter.h"
#include "Window.h"

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
        if (!PekanEngine::init(this))
        {
            PK_LOG_ERROR("Engine failed to initialize.", "Pekan");
            return false;
        }

        if (!_init())
        {
            return false;
        }

        // Initalize all layers
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
        // If this application doesn't have a specific FPS requirement,
        // then use VSync to automatically match FPS with monitor's refresh rate
        const bool useVSync = (m_fps <= 0.0);
        if (useVSync)
        {
            PekanEngine::getWindow().enableVSync();
        }
        FpsLimiter fpsLimiter(m_fps);

        Window& window = PekanEngine::getWindow();
        while (!window.shouldBeClosed())
        {
            glfwPollEvents();

            // Handle window resizing
            const glm::ivec2 frameBufferSize = window.getFrameBufferSize();
            glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

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

            // Swap buffers to show the new frame on screen.
            // If we are using VSync this function will automatically wait
            // the correct amount of time before the next screen update
            window.swapBuffers();
            // If we are NOT using VSync then we need to manually wait some amount of time to reach our target FPS
            if (!useVSync)
            {
                fpsLimiter.wait();
            }
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

    void PekanApplication::stopRunning()
    {
        PekanEngine::getWindow().setShouldBeClosed(true);
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