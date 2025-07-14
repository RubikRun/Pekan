#include "ColorEdit3Widget.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	ColorEdit3Widget::ColorEdit3Widget(glm::vec3 initialValue)
		: m_value(initialValue)
	{}
	ColorEdit3Widget::ColorEdit3Widget(const char* label)
		: m_label(label)
	{}
	ColorEdit3Widget::ColorEdit3Widget(const char* label, glm::vec3 initialValue)
		: m_label(label)
		, m_value(initialValue)
	{}

	void ColorEdit3Widget::render()
	{
		ImGui::ColorEdit3(m_label.c_str(), (float*)(&m_value));
	}

} // namespace GUI
} // namespace Pekan
