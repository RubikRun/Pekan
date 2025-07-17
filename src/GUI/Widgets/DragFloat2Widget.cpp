#include "DragFloat2Widget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	DragFloat2Widget::DragFloat2Widget()
		: m_id(GUI::generateWidgetId())
	{}
	DragFloat2Widget::DragFloat2Widget(glm::vec2 initialValue, float min, float max, float step)
		: m_value(initialValue)
		, m_min(min), m_max(max), m_step(step)
		, m_id(GUI::generateWidgetId())
	{}
	DragFloat2Widget::DragFloat2Widget(const char* label, float min, float max, float step)
		: m_label(label)
		, m_min(min), m_max(max), m_step(step)
		, m_id(GUI::generateWidgetId())
	{}
	DragFloat2Widget::DragFloat2Widget(const char* label, glm::vec2 initialValue, float min, float max, float step)
		: m_label(label)
		, m_value(initialValue)
		, m_min(min), m_max(max), m_step(step)
		, m_id(GUI::generateWidgetId())
	{}

	void DragFloat2Widget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		ImGui::DragFloat2(m_label.c_str(), (float*)(&m_value), m_step, m_min, m_max);
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan
