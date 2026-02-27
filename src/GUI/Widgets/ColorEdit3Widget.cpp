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
		m_valueChangedByUserSinceLastAccess = false;
		return m_value;
	}

	void ColorEdit3Widget::setValue(glm::vec3 value)
	{
		m_value = value;
		m_valueChangedByUserSinceLastAccess = false;
	}

	const std::string& ColorEdit3Widget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	void ColorEdit3Widget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		if (ImGui::ColorEdit3(m_label.c_str(), (float*)(&m_value)))
		{
			m_valueChangedByUserSinceLastAccess = true;
		}
	}

	bool ColorEdit3Widget::wasChangedByUserSinceLastAccess() const
	{
		return m_valueChangedByUserSinceLastAccess;
	}

} // namespace GUI
} // namespace Pekan
