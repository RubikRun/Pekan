#include "Demo04_GUIWindow.h"

using namespace Pekan::GUI;

namespace Demo
{

	static const std::vector<std::string> SHADER_NAMES =
	{
		"Standard Blend",
		"Two Points Blend 1",
		"Two Points Blend 2",
		"Two Points Blend 3"
	};

	bool Demo04_GUIWindow::init()
	{
		gui.backgroundColorWidget->create(this, { 0.45f, 0.55f, 0.60f });
		gui.positionWidget->create(this, "Position", -1.0f, 1.0f, 0.01f);
		gui.shaderWidget->create(this, "Shader", SHADER_NAMES);
		gui.enableShapesWidget->create(this, "Enable Shapes", true);
		gui.reverseTriangleOrientationWidget->create(this, "Reverse Triangle Orientation");
		gui.reversePolygonOrientationWidget->create(this, "Reverse Polygon Orientation");
		gui.enableFaceCullingWidget->create(this, "Enable Face Culling");

		return true;
	}

	GUIWindowProperties Demo04_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 300, 340 };
		props.name = "Textures & Shapes";
		return props;
	}

} // namespace Demo