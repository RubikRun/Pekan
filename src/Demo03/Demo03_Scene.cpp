#include "Demo03_Scene.h"

// TEMP: testing event system
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;

// TEMP: testing event system
using Pekan::KeyPressedEvent; using Pekan::KeyReleasedEvent;
using Pekan::MouseMovedEvent; using Pekan::MouseScrolledEvent; using Pekan::MouseButtonPressedEvent; using Pekan::MouseButtonReleasedEvent;
using Pekan::WindowResizedEvent; using Pekan::WindowClosedEvent;

namespace Demo
{

	bool Demo03_Scene::init()
	{
		m_snake.create();
		m_apple.create(m_snake);

        return true;
	}

	void Demo03_Scene::update()
	{
		m_snake.update();
		m_apple.update();

		if (m_snake.checkHeadOverlapsApple(m_apple))
		{
			m_apple.regenerate(m_snake);
			m_snake.grow();
		}
	}

	void Demo03_Scene::render()
	{
		PekanRenderer::clear();
		m_apple.render();
		m_snake.render();
	}

	void Demo03_Scene::exit()
	{
		m_snake.destroy();
		m_apple.destroy();
	}

    // TEMP: testing event system
    bool Demo03_Scene::onKeyPressed(KeyPressedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onKeyReleased(Pekan::KeyReleasedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onMouseMoved(Pekan::MouseMovedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onMouseScrolled(Pekan::MouseScrolledEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onMouseButtonPressed(Pekan::MouseButtonPressedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onMouseButtonReleased(Pekan::MouseButtonReleasedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onWindowResized(Pekan::WindowResizedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

    bool Demo03_Scene::onWindowClosed(Pekan::WindowClosedEvent& event)
    {
        PK_LOG_INFO(event, "Boris");
        return true;
    }

} // namespace Demo