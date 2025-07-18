#include "SliderIntWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void SliderIntWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void SliderIntWidget::create(GUIWindow* guiWindow, int initialValue, int min, int max)
	{
		Widget::create(guiWindow);
		m_value = initialValue;
		m_min = min;
		m_max = max;
	}
	void SliderIntWidget::create(GUIWindow* guiWindow, const char* label, int min, int max)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_min = min;
		m_max = max;
	}
	void SliderIntWidget::create(GUIWindow* guiWindow, const char* label, int initialValue, int min, int max)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_value = initialValue;
		m_min = min;
		m_max = max;
	}

	void SliderIntWidget::_render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		ImGui::SliderInt(m_label.c_str(), &m_value, m_min, m_max);
	}

} // namespace GUI
} // namespace Pekan
