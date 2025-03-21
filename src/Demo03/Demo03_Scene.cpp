#include "Demo03_Scene.h"

namespace Demo
{

    bool Demo03_Scene::init()
	{
		m_snake.init();
        return true;
	}

	void Demo03_Scene::update()
	{
		m_snake.update();
	}

	void Demo03_Scene::render()
	{
		m_snake.render();
	}

	void Demo03_Scene::exit()
	{
		m_snake.exit();
	}

} // namespace Demo