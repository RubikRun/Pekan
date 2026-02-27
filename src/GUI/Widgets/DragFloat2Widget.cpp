#include "DragFloat2Widget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pekan
{
namespace GUI
{

	void DragFloat2Widget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, glm::vec2 initialValue, float min, float max, float step, const std::string& format)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_step = step;
		m_format = format;
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, const char* label, float min, float max, float step, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_min = min;
		m_max = max;
		m_step = step;
		m_format = format;
	}
	void DragFloat2Widget::create(GUIWindow* guiWindow, const char* label, glm::vec2 initialValue, float min, float max, float step, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_step = step;
		m_format = format;
	}
	void DragFloat2Widget::destroy()
	{
		m_label.clear();
		m_value = { 0.0f, 0.0f };
		m_min = 0.0f;
		m_max = 1.0f;
		m_step = 0.01f;
		m_format = "%.3f";

		Widget::destroy();
	}

	glm::vec2 DragFloat2Widget::getValue() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		m_valueChangedByUserSinceLastAccess = false;
		return m_value;
	}

	void DragFloat2Widget::setValue(glm::vec2 value)
	{
		m_value = value;
		m_valueChangedByUserSinceLastAccess = false;
	}

	const std::string& DragFloat2Widget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	float DragFloat2Widget::getMin() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_min;
	}

	float DragFloat2Widget::getMax() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_max;
	}

	float DragFloat2Widget::getStep() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_step;
	}

	const std::string& DragFloat2Widget::getFormat() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_format;
	}

	void DragFloat2Widget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		if (ImGui::DragFloat2(m_label.c_str(), glm::value_ptr(m_value), m_step, m_min, m_max, m_format.c_str()))
		{
			m_valueChangedByUserSinceLastAccess = true;
		}
	}

	bool DragFloat2Widget::wasChangedByUserSinceLastAccess() const
	{
		return m_valueChangedByUserSinceLastAccess;
	}

} // namespace GUI
} // namespace Pekan
