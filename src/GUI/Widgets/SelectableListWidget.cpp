#include "SelectableListWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void SelectableListWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}

	void SelectableListWidget::destroy()
	{
		m_items.clear();
		m_selectedIndex = -1;
		Widget::destroy();
	}

	void SelectableListWidget::setItems(const std::vector<std::string>& items)
	{
		PK_ASSERT_QUICK(isValid());
		m_items = items;
		
		// Reset selection if it's out of bounds
		if (m_selectedIndex >= int(m_items.size()))
		{
			m_selectedIndex = -1;
		}
	}

	int SelectableListWidget::getSelectedIndex() const
	{
		PK_ASSERT_QUICK(isValid());
		return m_selectedIndex;
	}

	void SelectableListWidget::setSelectedIndex(int index)
	{
		PK_ASSERT_QUICK(isValid());
		m_selectedIndex = index;
	}

	void SelectableListWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());

		// Render each item as a selectable
		for (int i = 0; i < int(m_items.size()); i++)
		{
			const bool isSelected = (i == m_selectedIndex);

			// Render selectable and check if it was clicked
			if (ImGui::Selectable(m_items[i].c_str(), isSelected))
			{
				m_selectedIndex = i;
			}
		}
	}

} // namespace GUI
} // namespace Pekan
