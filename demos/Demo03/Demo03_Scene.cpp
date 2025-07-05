#include "Demo03_Scene.h"

#include "PekanApplication.h"
#include "Events/KeyEvents.h"
#include "RenderCommands.h"

using Pekan::KeyPressedEvent;
using Pekan::KeyCode;

using namespace Pekan::Graphics;

namespace Demo
{

	bool Demo03_Scene::init()
	{
		m_snake.create();
		m_apple.create(m_snake);

        return true;
	}

	void Demo03_Scene::update(double dt)
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
		RenderCommands::clear();
		m_apple.render();
		m_snake.render();
	}

	void Demo03_Scene::exit()
	{
		m_snake.destroy();
		m_apple.destroy();
	}

	bool Demo03_Scene::onKeyPressed(const Pekan::KeyPressedEvent& event)
	{
		// Make application close if escape key is pressed
		if (event.getKeyCode() == KeyCode::KEY_ESCAPE)
		{
			stopRunningApplication();
			return true;
		}
		return false;
	}

} // namespace Demo