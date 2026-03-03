#include "NewLineWidget.h"

#include "PekanLogger.h"

#include "imgui.h"

namespace Pekan
{
namespace GUI
{

	void NewLineWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void NewLineWidget::destroy()
	{
		Widget::destroy();
	}

	void NewLineWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::NewLine();
	}

} // namespace GUI
} // namespace Pekan
