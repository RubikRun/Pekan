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
		m_checkedChangedByUserSinceLastAccess = false;
		return m_isChecked;
	}

	void CheckboxWidget::setChecked(bool checked)
	{
		m_isChecked = checked;
		m_checkedChangedByUserSinceLastAccess = false;
	}

	const std::string& CheckboxWidget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void CheckboxWidget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		if (ImGui::Checkbox(m_label.c_str(), &m_isChecked))
		{
			m_checkedChangedByUserSinceLastAccess = true;
		}
	}

	bool CheckboxWidget::wasChangedByUserSinceLastAccess() const
	{
		return m_checkedChangedByUserSinceLastAccess;
	}

} // namespace GUI
} // namespace Pekan
