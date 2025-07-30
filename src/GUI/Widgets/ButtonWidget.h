#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A button widget allowing you to push it in GUI and cause some event.
	//
	// NOTE: Instances of this class MUST be owned by a ButtonWidget_Ptr
	class ButtonWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, const char* label);
		void destroy();

		bool isClicked() const;

		const std::string& getLabel() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The "is currently clicked" state of the button
		mutable bool m_isClicked = false;
	};

	typedef std::shared_ptr<ButtonWidget> ButtonWidget_Ptr;
	typedef std::shared_ptr<const ButtonWidget> ButtonWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan
