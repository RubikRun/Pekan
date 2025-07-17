#include "ColorEdit4Widget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	ColorEdit4Widget::ColorEdit4Widget()
		: m_id(GUI::generateWidgetId())
	{}
	ColorEdit4Widget::ColorEdit4Widget(glm::vec4 initialValue)
		: m_value(initialValue)
		, m_id(GUI::generateWidgetId())
	{}
	ColorEdit4Widget::ColorEdit4Widget(const char* label)
		: m_label(label)
		, m_id(GUI::generateWidgetId())
	{}
	ColorEdit4Widget::ColorEdit4Widget(const char* label, glm::vec4 initialValue)
		: m_label(label)
		, m_value(initialValue)
		, m_id(GUI::generateWidgetId())
	{}

	void ColorEdit4Widget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		ImGui::ColorEdit4(m_label.c_str(), (float*)(&m_value));
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan
