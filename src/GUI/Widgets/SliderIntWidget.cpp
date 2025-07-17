#include "SliderIntWidget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	SliderIntWidget::SliderIntWidget()
		: m_id(GUI::generateWidgetId())
	{}
	SliderIntWidget::SliderIntWidget(int initialValue, int min, int max)
		: m_value(initialValue)
		, m_min(min), m_max(max)
		, m_id(GUI::generateWidgetId())
	{}
	SliderIntWidget::SliderIntWidget(const char* label, int min, int max)
		: m_label(label)
		, m_min(min), m_max(max)
		, m_id(GUI::generateWidgetId())
	{}
	SliderIntWidget::SliderIntWidget(const char* label, int initialValue, int min, int max)
		: m_label(label)
		, m_value(initialValue)
		, m_min(min), m_max(max)
		, m_id(GUI::generateWidgetId())
	{}

	void SliderIntWidget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		ImGui::SliderInt(m_label.c_str(), &m_value, m_min, m_max);
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan
