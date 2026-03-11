#pragma once

#include "Widget.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace GUI
{

	// An enum containing the different ways a context menu can be triggered.
	enum class ContextMenuTriggerMode
	{
		// Context menu is triggered by right-clicking on the remaining empty space in the GUI window.
		// With this trigger mode, the context menu should be the last widget in the GUI window.
		EmptySpace,

		// Context menu is triggered by right-clicking on the widget
		// rendered immediately before the context menu widget in the GUI window.
		PreviousWidget,

		// Context menu is triggered by right-clicking anywhere in the GUI window.
		Window
	};

	// A right-click context menu widget.
	//
	// Displays a context menu when user right-clicks according to ContextMenuTriggerMode.
	// Contains a tree of menu items and supports arbitrary nesting.
	//
	// NOTE: Instances of this class MUST be owned by a ContextMenuWidget_Ptr
	class ContextMenuWidget : public Widget
	{
	public:

		// A single item/node of a context menu tree.
		// If children is empty, item is selectable by user.
		// If children is non-empty, item is a submenu.
		struct MenuItem
		{
			std::string label;
			std::vector<MenuItem> children;

			// Item ID returned when user selects this item.
			//
			// ATTENTION: For leaf items (items without children), this MUST be non-negative.
			//            Non-leaf items can have whatever ID because they are not selectable.
			int itemId = -1;
		};

		void create(GUIWindow* guiWindow, ContextMenuTriggerMode triggerMode);
		void create(GUIWindow* guiWindow, ContextMenuTriggerMode triggerMode, const std::vector<MenuItem>& items);
		void destroy();

		// Sets full context menu tree. Supports arbitrary nesting.
		void setMenuItems(const std::vector<MenuItem>& items);

		// Returns item ID of the item selected by user in current frame,
		// or -1 if no item was selected in current frame.
		int getSelectedItemId() const;

	private: /* functions */

		// Renders the context menu widget, if triggered by user right-clicking
		// on the specific area of the GUI window determined by the trigger mode.
		void _render() const override;

		// Renders the content of the context menu.
		// To be called once we know that the context menu is opened and should be rendered.
		// This happens when user right-clicks on a particular area of the GUI window,
		// determined by the trigger mode.
		void renderMenuContent() const;

	private: /* variables */

		// Context menu's trigger mode. Determines how the context menu is triggered.
		ContextMenuTriggerMode m_triggerMode = ContextMenuTriggerMode::EmptySpace;

		// List of root menu items.
		// Contains all items and submenus in a tree structure.
		std::vector<MenuItem> m_menuItems;

		// ID of item selected in current frame, or -1 if none
		mutable int m_selectedItemId = -1;
	};

	typedef std::shared_ptr<ContextMenuWidget> ContextMenuWidget_Ptr;
	typedef std::shared_ptr<const ContextMenuWidget> ContextMenuWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan
