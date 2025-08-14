#include "Demo08_GUIWindow.h"

#include "PekanLogger.h"

using namespace Pekan::GUI;

namespace Demo
{

	bool Demo08_GUIWindow::init()
	{
		gui.numberOfSpritesLabel->create(this, "Number Of Sprites");
		gui.numberOfSpritesWidget->create(this, "", 4000, 1, 10000);
		gui.mouseStrengthWidget->create(this, "Mouse Strength", 1.0f, 0.5f, 16.0f, "%.1f");
		gui.animSpeedWidget->create(this, "Animation Speed", 2.0f, 1.0f, 20.0f, "%.1f");

		gui.fpsDisplayWidget->create(this);

		return true;
	}

	GUIWindowProperties Demo08_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 360, 300 };
		props.name = "Demo08";
		return props;
	}

} // namespace Demo