#include "TextWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void TextWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void TextWidget::create(GUIWindow* guiWindow, const char* text)
	{
		Widget::create(guiWindow);
		m_text = text;
	}
	void TextWidget::destroy()
	{
		Widget::destroy();
		m_text = std::string();
	}

	void TextWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::Text(m_text.c_str());
	}

} // namespace GUI
} // namespace Pekan
