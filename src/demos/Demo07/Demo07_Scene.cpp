#include "Demo07_Scene.h"

#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanRenderer.h"

using namespace Pekan::Renderer;

namespace Demo
{

    bool Demo07_Scene::init()
	{
        return true;
	}

	void Demo07_Scene::update(double dt)
	{
	}

	void Demo07_Scene::render()
	{
		PekanRenderer::clear();
	}

	void Demo07_Scene::exit()
	{
	}

} // namespace Demo