#include "PekanApplication.h"
#include "Logger/PekanLogger.h"
#include "PekanEngine.h"
#include "Time/FpsLimiter.h"
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
            // Process all pending events, calling the handler function of each one.
            glfwPollEvents();
            // Process all reamining events - those that were not handled by their handler function,
            // and instead were added to the event queue.
            handleEventQueue();

            // Handle window resizing
            const glm::ivec2 frameBufferSize = window.getFrameBufferSize();
            glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

            const double deltaTime = m_deltaTimer.getDeltaTime();

            // Update all layers
            for (Layer* layer : m_layerStack)
            {
                if (layer)
                {
                    layer->update(deltaTime);
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
    // If no layer successfully handles the event, then the event is pushed into the event queue
    //
    // WARNING: Leaves the original event object in an invalid state. Make sure you DON'T use it after calling this function.
    template<typename EventT>
    static void _dispatchEvent(std::unique_ptr<EventT>& event, LayerStack& layerStack, EventQueue& eventQueue, bool (Layer::*onEventFunc)(EventT&))
    {
        for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
        {
            Layer* layer = *it;
            if (layer && (layer->*onEventFunc)(*event.get()))
            {
                break;
            }
        }
        // At this point no layer has handled the event, so add it to event queue
        eventQueue.push(std::move(event));
    }

    void PekanApplication::handleKeyEvent(KeyCode key, int scancode, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                std::unique_ptr<KeyPressedEvent> event = std::make_unique<KeyPressedEvent>(key, false);
                _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onKeyPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                std::unique_ptr<KeyReleasedEvent> event = std::make_unique<KeyReleasedEvent>(key);
                _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onKeyReleased);
                break;
            }
            case GLFW_REPEAT:
            {
                std::unique_ptr<KeyPressedEvent> event = std::make_unique<KeyPressedEvent>(key, true);
                _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onKeyPressed);
                break;
            }
        }
    }

    void PekanApplication::handleMouseMovedEvent(double xPos, double yPos)
    {
        std::unique_ptr<MouseMovedEvent> event = std::make_unique<MouseMovedEvent>(float(xPos), float(yPos));
        _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onMouseMoved);
    }

    void PekanApplication::handleMouseScrolledEvent(double xOffset, double yOffset)
    {
        std::unique_ptr<MouseScrolledEvent> event = std::make_unique<MouseScrolledEvent>(float(xOffset), float(yOffset));
        _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onMouseScrolled);
    }

    void PekanApplication::handleMouseButtonEvent(MouseButton button, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                std::unique_ptr<MouseButtonPressedEvent> event = std::make_unique<MouseButtonPressedEvent>(button);
                _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onMouseButtonPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                std::unique_ptr<MouseButtonReleasedEvent> event = std::make_unique<MouseButtonReleasedEvent>(button);
                _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onMouseButtonReleased);
                break;
            }
        }
    }

    void PekanApplication::handleWindowResizedEvent(int width, int height)
    {
        std::unique_ptr<WindowResizedEvent> event = std::make_unique<WindowResizedEvent>(width, height);
        _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onWindowResized);
    }

    void PekanApplication::handleWindowClosedEvent()
    {
        std::unique_ptr<WindowClosedEvent> event = std::make_unique<WindowClosedEvent>();
        _dispatchEvent(event, m_layerStack, m_eventQueue, &Layer::onWindowClosed);
    }

} // namespace Pekan