#include "DragFloat2Widget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void DragFloat2Widget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, glm::vec2 initialValue, float min, float max, float step)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_step = step;
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, const char* label, float min, float max, float step)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_min = min;
		m_max = max;
		m_step = step;
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, const char* label, glm::vec2 initialValue, float min, float max, float step)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_step = step;
	}

	void DragFloat2Widget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::DragFloat2(m_label.c_str(), (float*)(&m_value), m_step, m_min, m_max);
	}

} // namespace GUI
} // namespace Pekan
