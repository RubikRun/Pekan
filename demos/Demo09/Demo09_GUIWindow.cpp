#include "Demo09_GUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool Demo09_GUIWindow::init()
	{
		return true;
	}

	GUIWindowProperties Demo09_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 200, 400 };
		props.name = "Demo09";
		return props;
	}

} // namespace Demo
