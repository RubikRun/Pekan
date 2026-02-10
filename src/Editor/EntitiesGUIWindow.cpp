#include "EntitiesGUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

	bool EntitiesGUIWindow::_init()
	{
		return true;
	}

	GUIWindowProperties EntitiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entities";
		return props;
	}

} // namespace Editor
} // namespace Pekan
