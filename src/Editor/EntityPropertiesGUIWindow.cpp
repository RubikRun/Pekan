#include "EntityPropertiesGUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

	bool EntityPropertiesGUIWindow::init()
	{
		return true;
	}

	GUIWindowProperties EntityPropertiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entity Properties";
		return props;
	}

} // namespace Editor
} // namespace Pekan
