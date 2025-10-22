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

	void Demo09_GUIWindow::_render() const
	{
		// TEMP: remove this once TO-DO item 0071 is done.
		Graphics::RenderCommands::clear();
	}

} // namespace Demo
