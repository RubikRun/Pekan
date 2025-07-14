#include "TextWidget.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	TextWidget::TextWidget(const char* text)
		: m_text(text)
	{}

	void TextWidget::render()
	{
		ImGui::Text(m_text.c_str());
	}

} // namespace GUI
} // namespace Pekan
