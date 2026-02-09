#include "EditorScene.h"

#include "RenderState.h"
#include "PekanLogger.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;
using namespace Pekan;

namespace Pekan
{
namespace Editor
{

	bool EditorScene::_init()
	{
		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		return true;
	}

	void EditorScene::_exit()
	{
	}

	void EditorScene::update(double deltaTime)
	{
	}

} // namespace Editor
} // namespace Pekan
