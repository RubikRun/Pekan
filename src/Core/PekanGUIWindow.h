#pragma once

#include "Layer.h"

namespace Pekan
{

	// A base class for all Pekan GUI windows
	class PekanGUIWindow : public Layer
	{
	public:
		PekanGUIWindow(Pekan::PekanApplication* application) : Layer("gui_layer", application) {}
		PekanGUIWindow(const std::string& layerName, Pekan::PekanApplication* application) : Layer(layerName, application) {}

		virtual ~PekanGUIWindow() = default;

		void render() override final;

	private: /* functions*/

		virtual void _render() = 0;

		bool onKeyPressed(const KeyPressedEvent& event) override;
		bool onKeyReleased(const KeyReleasedEvent& event) override;
		bool onMouseMoved(const MouseMovedEvent& event) override;
		bool onMouseScrolled(const MouseScrolledEvent& event) override;
		bool onMouseButtonPressed(const MouseButtonPressedEvent& event) override;
		bool onMouseButtonReleased(const MouseButtonReleasedEvent& event) override;
	};

} // namespace Pekan