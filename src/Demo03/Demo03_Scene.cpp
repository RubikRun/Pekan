#include "Demo03_Scene.h"

#include "PekanApplication.h"
#include "Events/KeyEvent.h"

// TODO: remove this after we have Pekan enum for key codes
#include <GLFW/glfw3.h>

using Pekan::Renderer::PekanRenderer;

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

	bool Demo03_Scene::onKeyPressed(Pekan::KeyPressedEvent& event)
	{
		// Make application close if escape key is pressed
		if (event.getKeyCode() == GLFW_KEY_ESCAPE)
		{
			stopRunningApplication();
			return true;
		}
		return false;
	}

} // namespace Demo