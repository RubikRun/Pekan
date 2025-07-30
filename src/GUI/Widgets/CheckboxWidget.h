#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A checkbox widget allowing control over a boolean parameter in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a CheckboxWidget_Ptr
	class CheckboxWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, bool initialIsChecked);
		void create(GUIWindow* guiWindow, const char* label);
		void create(GUIWindow* guiWindow, const char* label, bool initialIsChecked);
		void destroy();

		bool isChecked() const;

		const std::string& getLabel() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The checked states of the checkbox
		mutable bool m_isChecked = false;
	};

	typedef std::shared_ptr<CheckboxWidget> CheckboxWidget_Ptr;
	typedef std::shared_ptr<const CheckboxWidget> CheckboxWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan
