#pragma once

#include "Widget.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace GUI
{

	// A widget displaying a list of selectable items.
	// Tracks which item is currently selected and allows querying selection changes.
	//
	// NOTE: Instances of this class MUST be owned by a SelectableListWidget_Ptr
	class SelectableListWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void destroy();

		// Sets the list of items to display
		void setItems(const std::vector<std::string>& items);

		// Returns the currently selected index, or -1 if nothing is selected.
		int getSelectedIndex() const;

		// Sets the selected index (-1 to clear selection)
		void setSelectedIndex(int index);

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// List of items to display
		std::vector<std::string> m_items;

		// Currently selected index (-1 if nothing is selected)
		mutable int m_selectedIndex = -1;
	};

	typedef std::shared_ptr<SelectableListWidget> SelectableListWidget_Ptr;
	typedef std::shared_ptr<const SelectableListWidget> SelectableListWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan
