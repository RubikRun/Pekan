#include "SliderFloatWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void SliderFloatWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void SliderFloatWidget::create(GUIWindow* guiWindow, float initialValue, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloatWidget::create(GUIWindow* guiWindow, const char* label, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloatWidget::create(GUIWindow* guiWindow, const char* label, float initialValue, float min, float max, const std::string& format)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
		m_min = min;
		m_max = max;
		m_format = format;
	}
	void SliderFloatWidget::destroy()
	{
		m_label.clear();
		m_value = 0.0f;
		m_min = 0.0f;
		m_max = 1.0f;
		m_format = "%.3f";
		Widget::destroy();
	}

	float SliderFloatWidget::getValue() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_value;
	}

	const std::string& SliderFloatWidget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	float SliderFloatWidget::getMin() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_min;
	}

	float SliderFloatWidget::getMax() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_max;
	}

	const std::string& SliderFloatWidget::getFormat() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_format;
	}

	void SliderFloatWidget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::SliderFloat(m_label.c_str(), &m_value, m_min, m_max, m_format.c_str());
	}

} // namespace GUI
} // namespace Pekan
