#include "SeparatorWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void SeparatorWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void SeparatorWidget::destroy()
	{
		Widget::destroy();
	}

	void SeparatorWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::Separator();
	}

} // namespace GUI
} // namespace Pekan
