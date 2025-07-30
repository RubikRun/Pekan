#include "ButtonWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void ButtonWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void ButtonWidget::create(GUIWindow* guiWindow, const char* label)
	{
		Widget::create(guiWindow);
		m_label = label;
	}
	void ButtonWidget::destroy()
	{
		m_label.clear();
		m_isClicked = false;
		Widget::destroy();
	}

	bool ButtonWidget::isClicked() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_isClicked;
	}

	const std::string& ButtonWidget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void ButtonWidget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		m_isClicked = ImGui::Button(m_label.c_str());
	}

} // namespace GUI
} // namespace Pekan
