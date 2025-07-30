#include "SliderFloat2Widget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pekan
{
namespace GUI
{

	void SliderFloat2Widget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void SliderFloat2Widget::create(GUIWindow* guiWindow, glm::vec2 initialValue, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloat2Widget::create(GUIWindow* guiWindow, const char* label, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloat2Widget::create(GUIWindow* guiWindow, const char* label, glm::vec2 initialValue, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloat2Widget::destroy()
	{
		m_label.clear();
		m_value = { 0.0f, 0.0f };
		m_min = 0.0f;
		m_max = 1.0f;
		m_format = "%.3f";
		Widget::destroy();
	}

	glm::vec2 SliderFloat2Widget::getValue() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_value;
	}

	const std::string& SliderFloat2Widget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	float SliderFloat2Widget::getMin() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_min;
	}

	float SliderFloat2Widget::getMax() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_max;
	}

	const std::string& SliderFloat2Widget::getFormat() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_format;
	}

	void SliderFloat2Widget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::SliderFloat2(m_label.c_str(), glm::value_ptr(m_value), m_min, m_max, m_format.c_str());
	}

} // namespace GUI
} // namespace Pekan
