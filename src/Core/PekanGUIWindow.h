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
	};

} // namespace Pekan