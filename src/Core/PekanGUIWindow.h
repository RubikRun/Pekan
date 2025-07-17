#pragma once

#include "Layer.h"
#include <glm/glm.hpp>

namespace Pekan
{

	// Properties of a GUI window
	struct GUIWindowProperties
	{
		// Size of window, in pixels
		glm::vec2 size = glm::vec2(200, 200);

		// Window's name, displayed on its top bar
		std::string name = "GUI window";
	};

	// A base class for all Pekan GUI windows
	class PekanGUIWindow : public Layer
	{
	public:

		PekanGUIWindow(Pekan::PekanApplication* application) : Layer("gui_layer", application) {}
		PekanGUIWindow(const std::string& layerName, Pekan::PekanApplication* application) : Layer(layerName, application) {}

		virtual ~PekanGUIWindow() = default;

		void render() override final;

	private: /* functions*/

		// To be implemented by derived classes to render specific widgets
		virtual void _render() = 0;

		// Can be implemented by derived classes to return specific properties of the GUI window
		virtual GUIWindowProperties getProperties() { return {}; }

		// Override these "on event" functions
		// so that they can mark the event as handled when ImGui captures the mouse/keyboard.
		// If we don't do that, events will be propagated "behind" the GUI layer even if they already took effect on the GUI.
		bool onKeyPressed(const KeyPressedEvent& event) override;
		bool onKeyReleased(const KeyReleasedEvent& event) override;
		bool onMouseMoved(const MouseMovedEvent& event) override;
		bool onMouseScrolled(const MouseScrolledEvent& event) override;
		bool onMouseButtonPressed(const MouseButtonPressedEvent& event) override;
		bool onMouseButtonReleased(const MouseButtonReleasedEvent& event) override;
	};

} // namespace Pekan