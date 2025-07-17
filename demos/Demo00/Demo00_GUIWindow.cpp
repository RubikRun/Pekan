#include "Demo00_GUIWindow.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool Demo00_GUIWindow::init()
	{
		gui.textWidget_backgroundColor = TextWidget("Background Color");
		gui.colorEditWidget = ColorEdit3Widget({ 0.30f, 0.55f, 0.60f });
		gui.textWidget_position = TextWidget("Position");
		gui.positionWidget = DragFloat2Widget({ 0.0f, 0.0f }, -1.0f, 1.0f, 0.01f);

		return true;
	}

	void Demo00_GUIWindow::_render()
	{
		gui.textWidget_backgroundColor.render();
		gui.colorEditWidget.render();
		gui.textWidget_position.render();
		gui.positionWidget.render();
	}

	GUIWindowProperties Demo00_GUIWindow::getProperties()
	{
		GUIWindowProperties props;
		props.size = { 200, 120 };
		props.name = "Demo00";
		return props;
	}

} // namespace Demo