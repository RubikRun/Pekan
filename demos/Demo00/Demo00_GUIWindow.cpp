#include "Demo00_GUIWindow.h"

#include "PekanLogger.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool Demo00_GUIWindow::_init()
	{
		gui.textWidget_backgroundColor->create(this, "Background Color");
		gui.colorEditWidget->create(this, { 0.30f, 0.55f, 0.60f });
		gui.textWidget_position->create(this, "Position");
		gui.positionWidget->create(this, { 0.0f, 0.0f }, -1.0f, 1.0f, 0.01f);

		return true;
	}

	glm::vec4 Demo00_GUIWindow::getClearColor() const
	{
		PK_ASSERT_QUICK(gui.colorEditWidget != nullptr);
		return glm::vec4(gui.colorEditWidget->getValue(), 1.0f);
	}

	glm::vec2 Demo00_GUIWindow::getPosition() const
	{
		PK_ASSERT_QUICK(gui.positionWidget != nullptr);
		return gui.positionWidget->getValue();
	}

	GUIWindowProperties Demo00_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 200, 120 };
		props.name = "Demo00";
		return props;
	}

} // namespace Demo