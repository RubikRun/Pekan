#pragma once

#include <memory>

namespace Pekan
{
namespace GUI
{

	class GUIWindow;
	class Widget;

	typedef std::shared_ptr<Widget> Widget_Ptr;
	typedef std::shared_ptr<const Widget> Widget_ConstPtr;

	// A base class for all GUI widgets
	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:

		// Creates widget in a given GUI window.
		// If you don't want your widget to belong to a specific GUI window, you can pass nullptr for guiWindow.
		void create(GUIWindow* guiWindow);
		// Destroys the widget
		void destroy();

		// Checks if widget is valid, meaning that it has been created and not yet destroyed
		bool isValid() const;

		// Hides the widget. Hidden widgets are not rendered and cannot be interacted with.
		void hide();
		// Unhides the widget.
		void unhide();
		// Returns true if the widget is currently hidden.
		bool isHidden() const;

		// Renders widget on current GUI window
		void render() const;

	protected: /* variables */

		// Widget's ID in the GUI subsystem
		int m_id = -1;

	private: /* functions */

		// To be implemented by derived classes to render a specific widget type
		virtual void _render() const = 0;

	private: /* variables */

		// When true, widget is not rendered and cannot be interacted with
		bool m_hidden = false;
	};

	typedef std::shared_ptr<Widget> Widget_Ptr;
	typedef std::shared_ptr<const Widget> Widget_ConstPtr;

} // namespace GUI
} // namespace Pekan
