#include "Demo03_Scene.h"

#include "Utils/PekanUtils.h"

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

} // namespace Demo