#include "ButtonWidget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	ButtonWidget::ButtonWidget()
		: m_id(GUI::generateWidgetId())
	{}
	ButtonWidget::ButtonWidget(const char* label)
		: m_label(label)
		, m_id(GUI::generateWidgetId())
	{}

	void ButtonWidget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		m_isClicked = ImGui::Button(m_label.c_str());
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan
