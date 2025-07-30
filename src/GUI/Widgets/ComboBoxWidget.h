#pragma once

#include "Widget.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace GUI
{

	// A widget that allows user to pick an item from a list of items
	// 
	// NOTE: Instances of this class MUST be owned by a ComboBoxWidget_Ptr
	class ComboBoxWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow, const std::vector<std::string>& items);
		void create(GUIWindow* guiWindow, const char* label, const std::vector<std::string>& items);
		void create(GUIWindow* guiWindow, int initialIndex, const std::vector<std::string>& items);
		void create(GUIWindow* guiWindow, const char* label, int initialIndex, const std::vector<std::string>& items);
		void destroy();

		// Returns index of currently selected item
		int getIndex() const;

		const std::string& getLabel() const;

		const std::vector<std::string>& getItems() const;

	private: /* functions */

		void _render() const override;

		// Updates the list of pointers to items names with current items names
		void updateItemsPointers();

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// Index of currently chosen item
		mutable int m_index = 0;

		// Names of items in the combo box
		std::vector<std::string> m_items;
		// Raw char pointers to items names.
		std::vector<const char*> m_itemsPointers;
	};

	typedef std::shared_ptr<ComboBoxWidget> ComboBoxWidget_Ptr;
	typedef std::shared_ptr<const ComboBoxWidget> ComboBoxWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan
