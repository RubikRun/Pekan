#include "CheckboxWidget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	CheckboxWidget::CheckboxWidget()
		: m_id(GUI::generateWidgetId())
	{}
	CheckboxWidget::CheckboxWidget(bool initialIsChecked)
		: m_isChecked(initialIsChecked)
		, m_id(GUI::generateWidgetId())
	{}
	CheckboxWidget::CheckboxWidget(const char* label)
		: m_label(label)
		, m_id(GUI::generateWidgetId())
	{}
	CheckboxWidget::CheckboxWidget(const char* label, bool initialIsChecked)
		: m_label(label)
		, m_isChecked(initialIsChecked)
		, m_id(GUI::generateWidgetId())
	{}

	void CheckboxWidget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		ImGui::Checkbox(m_label.c_str(), &m_isChecked);
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan
