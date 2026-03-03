#include "TextWidget.h"

#include "PekanLogger.h"

#include "imgui.h"

namespace Pekan
{
namespace GUI
{

	void TextWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void TextWidget::create(GUIWindow* guiWindow, const char* text)
	{
		Widget::create(guiWindow);
		m_text = text;
	}
	void TextWidget::destroy()
	{
		m_text.clear();
		Widget::destroy();
	}

	const std::string& TextWidget::getText() const
	{
		PK_ASSERT_QUICK(isValid());
		return m_text;
	}

	void TextWidget::setText(const char* text)
	{
		PK_ASSERT_QUICK(isValid());
		m_text = text;
	}

	void TextWidget::setText(const std::string& text)
	{
		PK_ASSERT_QUICK(isValid());
		m_text = text;
	}

	void TextWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::Text(m_text.c_str());
	}

} // namespace GUI
} // namespace Pekan
