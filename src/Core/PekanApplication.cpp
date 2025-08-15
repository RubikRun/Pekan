#include "PekanApplication.h"
#include "PekanLogger.h"
#include "PekanEngine.h"
#include "Time/FpsLimiter.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include <GLFW/glfw3.h>

namespace Pekan
{
    bool PekanApplication::init()
    {
        PK_ASSERT(!isValid(), "Trying to initialize a PekanApplication instance that is already initialized.", "Pekan");

        m_isInitialized = true;

        // Initialize Pekan
        if (!PekanEngine::init(this))
        {
            PK_LOG_ERROR("PekanEngine failed to initialize.", "Pekan");
            return false;
        }

        // Initialize derived application
        if (!_init())
        {
            return false;
        }
        // Fill layer stack with derived application's layers
        if (!_fillLayerStack(m_layerStack))
        {
            return false;
        }

        // Initalize all layers of the layer stack
        m_layerStack.initAll();

        return true;
    }

    void PekanApplication::run()
	{
        PK_ASSERT(isValid(), "Trying to run a PekanApplication that is not yet initialized.", "Pekan");

        const ApplicationProperties properties = getProperties();
        const double fps = properties.fps;
        const bool useVSync = properties.useVSync;

        // If there is no target FPS and derived application wants to use VSync, then enable VSync
        if (fps <= 0.0 && useVSync)
        {
            PekanEngine::s_window.enableVSync();
        }
        // Create an FPS limiter with our target FPS
        FpsLimiter fpsLimiter(fps);

        Window& window = PekanEngine::s_window;
        while (!window.shouldBeClosed())
        {
            // Process all pending events, calling the handler function of each one.
            glfwPollEvents();
            // Process all remaining events - those that were not handled by their handler function,
            // and instead were added to the event queue.
            handleEventQueue();

            // Handle window resizing
            const glm::ivec2 frameBufferSize = window.getFrameBufferSize();
            glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

            // Get delta time - time passed since last frame
            const double deltaTime = m_deltaTimer.getDeltaTime();

            // Update and render all layers of the layer stack
            m_layerStack.updateAll(deltaTime);
            m_layerStack.renderAll();

            // Swap buffers to show the new frame on screen.
            // If we are using VSync this function will automatically wait
            // the correct amount of time before the next screen update.
            window.swapBuffers();
            // If there is a target FPS, then we need to manually wait some amount of time
            if (fps > 0.0)
            {
                fpsLimiter.wait();
            }
        }
	}

    void PekanApplication::exit()
    {
        PK_ASSERT(isValid(), "Trying to exit a PekanApplication that is not yet initialized.", "Pekan");

        // Exit all layers of the layer stack
        m_layerStack.exitAll();

        // Exit engine
        PekanEngine::exit();

        m_isInitialized = false;
    }

    void PekanApplication::registerEventListener(const std::shared_ptr<EventListener>& listener)
    {
        PK_ASSERT(isValid(), "Trying to register an event listener in a PekanApplication that is not yet initialized.", "Pekan");

        if (listener == nullptr)
        {
            PK_LOG_ERROR("Trying to register a NULL event listener in PekanApplication. It will be ignored.", "Pekan");
            return;
        }
        m_eventListeners.push_back(listener);
    }

    void PekanApplication::unregisterEventListener(const std::shared_ptr<EventListener>& listener)
    {
        PK_ASSERT(isValid(), "Trying to unregister an event listener from a PekanApplication that is not yet initialized.", "Pekan");

        m_eventListeners.erase
        (
            std::remove_if
            (
                m_eventListeners.begin(), m_eventListeners.end(),
                [&listener](const std::weak_ptr<EventListener>& otherListener)
                {
                    // Check if both pointers point to the same object
                    return !otherListener.owner_before(listener) && !listener.owner_before(otherListener);
                }
            ),
            m_eventListeners.end()
        );
    }

    void PekanApplication::stopRunning()
    {
        PK_ASSERT(isValid(), "Trying to stop running a PekanApplication that is not yet initialized.", "Pekan");

        // Set window's "should be closed" state to true,
        // so that the main loop is stopped on the next iteration.
        PekanEngine::s_window.setShouldBeClosed(true);
    }

    // Sends an event of a given type to layers of the layer stack,
    // one by one, until a layer successfully handles the event.
    // If no layer successfully handles the event, then the event is sent to all registered event listeners.
    // If event is still not handled, it will be pushed to the event queue.
    //
    // WARNING: Leaves the original event object in an invalid state. Make sure you DON'T use it after calling this function.
    template<typename EventT>
    static void _dispatchEvent
    (
        std::unique_ptr<EventT>& event,
        LayerStack& layerStack,
        std::vector<std::weak_ptr<EventListener>> eventListeners,
        EventQueue& eventQueue,
        bool (EventListener::*onEventFunc)(const EventT&)
    )
    {
        // Dispatch event to the layer stack
        if (layerStack.dispatchEvent(event, onEventFunc))
        {
            return;
        }

        bool handled = false;

        // If event was not handled by the layer stack,
        // call the onEventFunc on all registered event listeners
        for (auto it = eventListeners.begin(); it != eventListeners.end(); )
        {
            if (auto listener = it->lock())
            {
                if ((listener.get()->*onEventFunc)(*event.get()))
                {
                    handled = true;
                }
                ++it;
            }
            else
            {
                // Listener is expired, so remove it
                it = eventListeners.erase(it);
            }
        }

        // If event is still not handled, add it to event queue
        if (!handled)
        {
            eventQueue.push(std::move(event));
        }
    }

    void PekanApplication::handleKeyEvent(KeyCode key, int scancode, int action, int mods)
    {
        PK_ASSERT(isValid(), "Trying to handle key event in a PekanApplication that is not yet initialized.", "Pekan");

        switch (action)
        {
            case GLFW_PRESS:
            {
                std::unique_ptr<KeyPressedEvent> event = std::make_unique<KeyPressedEvent>(key, false);
                _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onKeyPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                std::unique_ptr<KeyReleasedEvent> event = std::make_unique<KeyReleasedEvent>(key);
                _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onKeyReleased);
                break;
            }
            case GLFW_REPEAT:
            {
                std::unique_ptr<KeyPressedEvent> event = std::make_unique<KeyPressedEvent>(key, true);
                _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onKeyPressed);
                break;
            }
        }
    }

    void PekanApplication::handleMouseMovedEvent(double xPos, double yPos)
    {
        PK_ASSERT(isValid(), "Trying to handle a mouse-moved event in a PekanApplication that is not yet initialized.", "Pekan");

        std::unique_ptr<MouseMovedEvent> event = std::make_unique<MouseMovedEvent>(float(xPos), float(yPos));
        _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onMouseMoved);
    }

    void PekanApplication::handleMouseScrolledEvent(double xOffset, double yOffset)
    {
        PK_ASSERT(isValid(), "Trying to handle a mouse-scrolled event in a PekanApplication that is not yet initialized.", "Pekan");

        std::unique_ptr<MouseScrolledEvent> event = std::make_unique<MouseScrolledEvent>(float(xOffset), float(yOffset));
        _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onMouseScrolled);
    }

    void PekanApplication::handleMouseButtonEvent(MouseButton button, int action, int mods)
    {
        PK_ASSERT(isValid(), "Trying to handle a mouse button event in a PekanApplication that is not yet initialized.", "Pekan");

        switch (action)
        {
            case GLFW_PRESS:
            {
                std::unique_ptr<MouseButtonPressedEvent> event = std::make_unique<MouseButtonPressedEvent>(button);
                _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onMouseButtonPressed);
                break;
            }
            case GLFW_RELEASE:
            {
                std::unique_ptr<MouseButtonReleasedEvent> event = std::make_unique<MouseButtonReleasedEvent>(button);
                _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onMouseButtonReleased);
                break;
            }
        }
    }

    void PekanApplication::handleWindowResizedEvent(int width, int height)
    {
        PK_ASSERT(isValid(), "Trying to handle a window-resized event in a PekanApplication that is not yet initialized.", "Pekan");

        std::unique_ptr<WindowResizedEvent> event = std::make_unique<WindowResizedEvent>(width, height);
        _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onWindowResized);
    }

    void PekanApplication::handleWindowClosedEvent()
    {
        PK_ASSERT(isValid(), "Trying to handle a window-closed event in a PekanApplication that is not yet initialized.", "Pekan");

        std::unique_ptr<WindowClosedEvent> event = std::make_unique<WindowClosedEvent>();
        _dispatchEvent(event, m_layerStack, m_eventListeners, m_eventQueue, &EventListener::onWindowClosed);
    }

} // namespace Pekan