#include "NewLineWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
