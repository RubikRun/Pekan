#include "ColorEdit4Widget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void ColorEdit4Widget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void ColorEdit4Widget::create(GUIWindow* guiWindow, glm::vec4 initialValue)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
	}
	void ColorEdit4Widget::create(GUIWindow* guiWindow, const char* label)
	{
		Widget::create(guiWindow);
		m_label = label;
	}
	void ColorEdit4Widget::create(GUIWindow* guiWindow, const char* label, glm::vec4 initialValue)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
	}
	void ColorEdit4Widget::destroy()
	{
		m_label.clear();
		m_value = { 1.0f, 1.0f, 1.0f, 1.0f };
		Widget::destroy();
	}

	glm::vec4 ColorEdit4Widget::getValue() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_value;
	}

	const std::string& ColorEdit4Widget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void ColorEdit4Widget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::ColorEdit4(m_label.c_str(), (float*)(&m_value));
	}

} // namespace GUI
} // namespace Pekan
