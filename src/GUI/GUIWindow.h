#pragma once

#include "Layer.h"
#include <glm/glm.hpp>

#include "Widget.h"

#include <vector>

namespace Pekan
{
namespace GUI
{
	// Properties of a GUI window
	struct GUIWindowProperties
	{
		// Size of window, in pixels
		glm::vec2 size = glm::vec2(200, 200);

		// Window's name, displayed on its top bar
		std::string name = "GUI window";
	};

	// A base class for all GUI windows in Pekan
	class GUIWindow : public Layer
	{
	public:

		GUIWindow(Pekan::PekanApplication* application) : Layer(application) {}
		virtual ~GUIWindow() = default;

		void exit() override final;
		void render() const  override final;

		// Adds a widget to the GUI window.
		// Widgets added here will be automatically rendered when the GUI window is rendred
		// in the order in which they were added,
		// and they will be automatically destroyed when the GUI window is destroyed.
		//
		// NOTE: If you want finer control over the rendering of your widgets,
		// there is no need to add them here,
		// you can override _render() and render them manually there.
		void addWidget(const Widget_Ptr& widget);

		// Can be implemented by derived classes if they want a non-default name
		// for the layer of their GUI window
		virtual inline std::string getLayerName() const override { return "gui_layer"; }

	private: /* functions*/

		// Can be implemented by derived classes with specific exiting functionality.
		virtual void _exit() {};

		// Can be implemented by derived classes with specific render functionality.
		virtual void _render() const {}

		// Can be implemented by derived classes to return specific properties of the GUI window
		virtual GUIWindowProperties getProperties() const { return {}; }

		// Override these "on event" functions
		// so that they can mark the event as handled when ImGui captures the mouse/keyboard.
		// If we don't do that, events will be propagated "behind" the GUI layer even if they already took effect on the GUI.
		bool onKeyPressed(const KeyPressedEvent& event) override;
		bool onKeyReleased(const KeyReleasedEvent& event) override;
		bool onMouseMoved(const MouseMovedEvent& event) override;
		bool onMouseScrolled(const MouseScrolledEvent& event) override;
		bool onMouseButtonPressed(const MouseButtonPressedEvent& event) override;
		bool onMouseButtonReleased(const MouseButtonReleasedEvent& event) override;

	private: /* variables */

		// Widgets making up the GUI inside the window.
		std::vector<Widget_Ptr> m_widgets;
	};

} // namespace GUI
} // namespace Pekan