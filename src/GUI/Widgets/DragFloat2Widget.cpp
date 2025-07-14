#include "DragFloat2Widget.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	DragFloat2Widget::DragFloat2Widget(glm::vec2 initialValue, float min, float max, float step)
		: m_value(initialValue)
		, m_min(min), m_max(max), m_step(step)
	{}
	DragFloat2Widget::DragFloat2Widget(const char* label, float min, float max, float step)
		: m_label(label)
		, m_min(min), m_max(max), m_step(step)
	{}
	DragFloat2Widget::DragFloat2Widget(const char* label, glm::vec2 initialValue, float min, float max, float step)
		: m_label(label)
		, m_value(initialValue)
		, m_min(min), m_max(max), m_step(step)
	{}

	void DragFloat2Widget::render()
	{
		ImGui::DragFloat2(m_label.c_str(), (float*)(&m_value), m_step, m_min, m_max);
	}

} // namespace GUI
} // namespace Pekan
