#include "PekanEngine.h"

#include "PekanLogger.h"
#include "PekanApplication.h"
#include "SubsystemManager.h"

namespace Pekan
{

    Window PekanEngine::s_window;
    PekanApplication* PekanEngine::s_application = nullptr;
    bool PekanEngine::isInitialized = false;

    bool PekanEngine::init(PekanApplication* application)
    {
        if (isInitialized)
        {
            PK_LOG_ERROR("Trying to initialize the engine more than once.", "Pekan");
            return false;
        }
        if (s_application != nullptr)
        {
            PK_LOG_ERROR("Multiple applications are trying to initialize the engine. Pekan supports only one application at a time.", "Pekan");
            return false;
        }
        if (application == nullptr)
        {
            PK_LOG_ERROR("A null application is given when trying to initialize the engine.", "Pekan");
            return false;
        }
        s_application = application;

        const ApplicationProperties properties = application->getProperties();
        if (!s_window.create(properties))
        {
            return false;
        }

        SubsystemManager::initAll();

        isInitialized = true;
        return true;
    }

    void PekanEngine::exit()
    {
        if (!isInitialized)
        {
            PK_LOG_ERROR("Trying to exit engine but engine is not yet initialized.", "Pekan");
            return;
        }
        s_window.destroy();

        SubsystemManager::exitAll();

        isInitialized = false;
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