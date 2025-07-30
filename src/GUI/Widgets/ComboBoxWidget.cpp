#include "ComboBoxWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{
	void ComboBoxWidget::create(GUIWindow* guiWindow, const std::vector<std::string>& items)
	{
		Widget::create(guiWindow);
		m_items = items;
		updateItemsPointers();
	}
	void ComboBoxWidget::create(GUIWindow* guiWindow, const char* label, const std::vector<std::string>& items)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_items = items;
		updateItemsPointers();
	}
	void ComboBoxWidget::create(GUIWindow* guiWindow, int initialIndex, const std::vector<std::string>& items)
	{
		Widget::create(guiWindow);
		m_index = initialIndex;
		m_items = items;
		updateItemsPointers();
	}
	void ComboBoxWidget::create(GUIWindow* guiWindow, const char* label, int initialIndex, const std::vector<std::string>& items)
	{
		Widget::create(guiWindow);
		m_label = label;
		m_index = initialIndex;
		m_items = items;
		updateItemsPointers();
	}
	void ComboBoxWidget::destroy()
	{
		m_label.clear();
		m_index = 0;
		m_items.clear();
		m_itemsPointers.clear();
		Widget::destroy();
	}

	int ComboBoxWidget::getIndex() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_index;
	}

	const std::string& ComboBoxWidget::getLabel() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_label;
	}

	const std::vector<std::string>& ComboBoxWidget::getItems() const
	{
		PK_ASSERT_QUICK(m_id >= 0);
		return m_items;
	}

	void ComboBoxWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());
		ImGui::Combo(m_label.c_str(), &m_index, m_itemsPointers.data(), m_itemsPointers.size());
		ImGui::Separator();
	}

	void ComboBoxWidget::updateItemsPointers()
	{
		m_itemsPointers.resize(m_items.size());
		for (size_t i = 0; i < m_itemsPointers.size(); ++i)
		{
			m_itemsPointers[i] = m_items[i].c_str();
		}
	}

} // namespace GUI
} // namespace Pekan
