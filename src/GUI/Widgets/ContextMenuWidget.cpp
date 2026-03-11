#include "ContextMenuWidget.h"

#include "PekanLogger.h"

#include "imgui.h"

namespace Pekan
{
namespace GUI
{

	// Checks if all leaf items in the given tree of menu items have non-negative item IDs
	static bool hasValidLeafItemIds(const std::vector<ContextMenuWidget::MenuItem>& items)
	{
		for (const ContextMenuWidget::MenuItem& item : items)
		{
			if (item.children.empty())
			{
				if (item.itemId < 0)
				{
					return false;
				}
			}
			else
			{
				if (!hasValidLeafItemIds(item.children))
				{
					return false;
				}
			}
		}
		return true;
	}

	// Renders a tree of menu items as a context menu
	static void renderMenuItems(const std::vector<ContextMenuWidget::MenuItem>& items, int& selectedItemId)
	{
		for (const ContextMenuWidget::MenuItem& item : items)
		{
			if (item.children.empty())
			{
				if (ImGui::MenuItem(item.label.c_str()))
				{
					selectedItemId = item.itemId;
				}
			}
			else
			{
				if (ImGui::BeginMenu(item.label.c_str()))
				{
					renderMenuItems(item.children, selectedItemId);
					ImGui::EndMenu();
				}
			}
		}
	}

	void ContextMenuWidget::create(GUIWindow* guiWindow, ContextMenuTriggerMode triggerMode)
	{
		Widget::create(guiWindow);
		m_triggerMode = triggerMode;
	}

	void ContextMenuWidget::create(GUIWindow* guiWindow, ContextMenuTriggerMode triggerMode, const std::vector<MenuItem>& items)
	{
		Widget::create(guiWindow);
		m_triggerMode = triggerMode;
		setMenuItems(items);
	}

	void ContextMenuWidget::destroy()
	{
		m_triggerMode = ContextMenuTriggerMode::EmptySpace;
		m_menuItems.clear();
		m_selectedItemId = -1;
		Widget::destroy();
	}

	void ContextMenuWidget::setMenuItems(const std::vector<MenuItem>& items)
	{
		PK_ASSERT_QUICK(isValid());
		PK_ASSERT(hasValidLeafItemIds(items), "ContextMenuWidget leaf menu items must have non-negative itemId values.", "Pekan");
		m_menuItems = items;
	}

	int ContextMenuWidget::getSelectedItemId() const
	{
		PK_ASSERT_QUICK(isValid());
		return m_selectedItemId;
	}

	void ContextMenuWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());

		// Clear selection from last frame
		m_selectedItemId = -1;

		switch (m_triggerMode)
		{
			case ContextMenuTriggerMode::EmptySpace:
			{
				// Get size of remaining available space in the GUI window.
				// "Available" here means empty space that doesn't have a widget on it yet.
				const ImVec2 availableSize = ImGui::GetContentRegionAvail();
				if (availableSize.x > 0.0f && availableSize.y > 0.0f)
				{
					// Render an invisible button covering all available space.
					// This is needed because BeginPopupContextItem() uses the last rendered item
					// as the trigger area for the context menu - user has to right-click
					// on the last rendered item to open the context menu.
					ImGui::InvisibleButton("##context_menu_area", availableSize);
					if (ImGui::BeginPopupContextItem())
					{
						renderMenuContent();
						ImGui::EndPopup();
					}
				}
				break;
			}
			case ContextMenuTriggerMode::PreviousWidget:
			{
				// BeginPopupContextItem() already uses the last rendered item as the trigger area
				// for the context menu, so nothing additional to do.
				if (ImGui::BeginPopupContextItem())
				{
					renderMenuContent();
					ImGui::EndPopup();
				}
				break;
			}
			case ContextMenuTriggerMode::Window:
			{
				// BeginPopupContextWindow() opens the context menu
				// if user right-clicks anywhere on the GUI window.
				if (ImGui::BeginPopupContextWindow())
				{
					renderMenuContent();
					ImGui::EndPopup();
				}
				break;
			}
		}
	}

	void ContextMenuWidget::renderMenuContent() const
	{
		renderMenuItems(m_menuItems, m_selectedItemId);
	}

} // namespace GUI
} // namespace Pekan
