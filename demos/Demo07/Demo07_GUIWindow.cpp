#include "Demo07_GUIWindow.h"

#define PI 3.14159265359f

using namespace Pekan::GUI;

namespace Demo
{

	constexpr float CAMERA_SIZE = 10.0f;

	constexpr char* const SHAPE_TYPE_NAMES[6] =
	{
		"Rectangle",
		"Circle",
		"Circle Static",
		"Triangle",
		"Polygon",
		"Line"
	};

	static glm::vec2 SHAPE_TYPE_INITIAL_POSITIONS[6] =
	{
		{ 1.5f, 1.0f },
		{ -1.5f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.5f, -1.0f },
		{ -1.5f, -2.0f },
		{ -1.5f, -2.0f }
	};

	bool Demo07_GUIWindow::init()
	{
		for (size_t i = 0; i < 6; ++i)
		{
			gui.shapeTypeWidgets[i].label->create(this, SHAPE_TYPE_NAMES[i]);
			gui.shapeTypeWidgets[i].separator->create(this);
			gui.shapeTypeWidgets[i].positionWidget->create(this, "Position", SHAPE_TYPE_INITIAL_POSITIONS[i], -CAMERA_SIZE, CAMERA_SIZE);
			gui.shapeTypeWidgets[i].rotationWidget->create(this, "Rotation", 0.0f, -PI, PI * 2.0f);
			gui.shapeTypeWidgets[i].scaleWidget->create(this, "Scale", { 1.0f, 1.0f }, -3.0f, 3.0f);

			if (i < 5)
			{
				gui.newLineWidgets[i]->create(this);
			}
		}

		return true;
	}

	GUIWindowProperties Demo07_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 300, 800 };
		props.name = "Demo07";
		return props;
	}

} // namespace Demo