#include "Demo02_GUIWindow.h"
#include "PekanLogger.h"
#include "Demo02_Scene.h"
#include "PekanEngine.h"

#include "glm/glm.hpp"

using namespace Pekan::GUI;
using Pekan::PekanEngine;

namespace Demo
{

	constexpr float COLOR_DULLNESS = 0.9f;

	constexpr glm::vec3 INITIAL_COLORS[6] =
	{
		{ COLOR_DULLNESS, 0.0f, 0.0f },
		{ 0.0f, COLOR_DULLNESS, 0.0f },
		{ 0.0f, 0.0f, COLOR_DULLNESS },
		{ COLOR_DULLNESS, COLOR_DULLNESS, 0.0f },
		{ COLOR_DULLNESS, 0.0f, COLOR_DULLNESS },
		{ 0.0f, COLOR_DULLNESS, COLOR_DULLNESS }
	};

	bool Demo02_GUIWindow::_init()
	{
		for (size_t i = 0; i < 6; i++)
		{
			const std::string colorLabel = std::string("Color ") + std::to_string(i + 1);
			gui.colorEditWidgets[i]->create(this, colorLabel.c_str(), INITIAL_COLORS[i]);
		}
		gui.separatorWidget->create(this);
		gui.rotationWidget->create(this, "Rotation", 0.0f, -180.0f, 180.0f);
		gui.fovWidget->create(this, "Field Of View", 30.0f, 10.0f, 100.0f);
		gui.cameraDistWidget->create(this, "Camera Distance", 2.5f, 0.5f, 10.0f);
		gui.hideFourthFaceWidget->create(this, "Hide Fourth Face", false);

		return true;
	}

	GUIWindowProperties Demo02_GUIWindow::getProperties() const
	{
		static const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();

		GUIWindowProperties props;
		props.size = { 380, windowSize.y };
		props.name = "Cube";
		return props;
	}

} // namespace Demo