#include "PekanEngine.h"

#include "PekanLogger.h"
#include "PekanApplication.h"
#include "SubsystemManager.h"

#include <iostream>

namespace Pekan
{

    Window PekanEngine::s_window;
    PekanApplication* PekanEngine::s_application = nullptr;
    bool PekanEngine::s_isInitialized = false;

    bool PekanEngine::init(PekanApplication* application)
    {
        if (s_isInitialized)
        {
            PK_LOG_ERROR("Trying to initialize PekanEngine but it's already initialized.", "Pekan");
            return false;
        }
        if (s_application != nullptr)
        {
            PK_LOG_ERROR("Multiple applications are trying to initialize PekanEngine. Pekan supports only one application at a time.", "Pekan");
            return false;
        }
        if (application == nullptr)
        {
            PK_LOG_ERROR("A null application is trying to initialize PekanEngine.", "Pekan");
            return false;
        }
        s_application = application;

        // Get application's properties
        const ApplicationProperties properties = application->getProperties();
        // Create a window with application's properties
        if (!s_window.create(properties))
        {
            return false;
        }

        std::cout << "Welcome to Pekan v0.2!" << std::endl;

        // Initialize all subsystems
        SubsystemManager::initAll();

        s_isInitialized = true;
        return true;
    }

    void PekanEngine::exit()
    {
        if (!s_isInitialized)
        {
            PK_LOG_ERROR("Trying to exit PekanEngine but it's not yet initialized.", "Pekan");
            return;
        }
        s_window.destroy();

        // Exit all subsystems
        SubsystemManager::exitAll();

        s_isInitialized = false;
    }

    bool PekanEngine::isKeyPressed(KeyCode key)
    {
        return s_window.isKeyPressed(key);
    }

    bool PekanEngine::isKeyReleased(KeyCode key)
    {
        return s_window.isKeyReleased(key);
    }

    bool PekanEngine::isKeyRepeating(KeyCode key)
    {
        return s_window.isKeyRepeating(key);
    }

    glm::vec2 PekanEngine::getMousePosition()
    {
        return s_window.getMousePosition();
    }

    glm::vec2 PekanEngine::getMousePositionNdc()
    {
        const glm::vec2 mousePosition = getMousePosition();
        const glm::vec2 windowSize = s_window.getSize();
        const glm::vec2 mousePositionNdc =
        {
            (mousePosition.x / windowSize.x) * 2.0f - 1.0f,
            1.0f - (mousePosition.y / windowSize.y) * 2.0f
        };
        return mousePositionNdc;
    }

    bool PekanEngine::isMouseButtonPressed(MouseButton button)
    {
        return s_window.isMouseButtonPressed(button);
    }

    bool PekanEngine::isMouseButtonReleased(MouseButton button)
    {
        return s_window.isMouseButtonReleased(button);
    }

} // namespace Pekan