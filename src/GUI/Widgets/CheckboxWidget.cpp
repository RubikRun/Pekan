#include "CheckboxWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void CheckboxWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void CheckboxWidget::create(GUIWindow* guiWindow, bool initialIsChecked)
	{
		Widget::create(guiWindow);
		m_isChecked = initialIsChecked;
	}
	void CheckboxWidget::create(GUIWindow* guiWindow, const char* label)
	{
		Widget::create(guiWindow);
		m_label = label;
	}
	void CheckboxWidget::create(GUIWindow* guiWindow, const char* label, bool initialIsChecked)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_isChecked = initialIsChecked;
	}
	void CheckboxWidget::destroy()
	{
		m_label.clear();
		m_isChecked = false;
		Widget::destroy();
	}

	bool CheckboxWidget::isChecked() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_isChecked;
	}

	const std::string& CheckboxWidget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void CheckboxWidget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::Checkbox(m_label.c_str(), &m_isChecked);
	}

} // namespace GUI
} // namespace Pekan
