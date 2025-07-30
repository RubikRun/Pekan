#include "ColorEdit3Widget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void ColorEdit3Widget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void ColorEdit3Widget::create(GUIWindow* guiWindow, glm::vec3 initialValue)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
	}
	void ColorEdit3Widget::create(GUIWindow* guiWindow, const char* label)
	{
		Widget::create(guiWindow);
		m_label = label;
	}
	void ColorEdit3Widget::create(GUIWindow* guiWindow, const char* label, glm::vec3 initialValue)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
	}
	void ColorEdit3Widget::destroy()
	{
		m_label.clear();
		m_value = { 1.0f, 1.0f, 1.0f };
		Widget::destroy();
	}

	glm::vec3 ColorEdit3Widget::getValue() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_value;
	}

	const std::string& ColorEdit3Widget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void ColorEdit3Widget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::ColorEdit3(m_label.c_str(), (float*)(&m_value));
	}

} // namespace GUI
} // namespace Pekan
