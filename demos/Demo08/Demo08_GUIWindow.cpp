#include "Demo08_GUIWindow.h"

#include "PekanLogger.h"

using namespace Pekan::GUI;

namespace Demo
{

	bool Demo08_GUIWindow::init()
	{
		gui.numberOfSpritesLabel->create(this, "Number Of Sprites");
		gui.numberOfSpritesWidget->create(this, "", 100, 1, 1000);

		gui.fpsDisplayWidget->create(this);

		return true;
	}

	GUIWindowProperties Demo08_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 300, 300 };
		props.name = "Demo08";
		return props;
	}

} // namespace Demo