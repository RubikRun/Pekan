#include "Demo09_GUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool Demo09_GUIWindow::init()
	{
		gui.ppsLabel->create(this, "Post-Processing Shader");
		gui.ppsComboBoxWidget->create(this, { "Identity", "Sharpen", "Blur", "Edge Detection", "Emboss" });

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
