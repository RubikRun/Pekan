#include "Demo05_GUIWindow.h"

using namespace Pekan::GUI;

namespace Demo
{

	bool Demo05_GUIWindow::init()
	{
		gui.backgroundColorWidget->create(this, "Background Color", { 0.1f, 0.1f, 0.1f });
		gui.lineThicknessWidget->create(this, "Line Thickness", 0.0005f, 0.00001f, 0.001f, "%.5f");

		return true;
	}

	GUIWindowProperties Demo05_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 120 };
		props.name = "Demo05";
		return props;
	}

} // namespace Demo