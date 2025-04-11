#pragma once

#include "Layer.h"

namespace Pekan
{

	// A base class for all Pekan GUI windows
	class PekanGUIWindow : public Layer
	{
	public:
		PekanGUIWindow() : Layer("gui_layer") {}
		PekanGUIWindow(const std::string& layerName) : Layer(layerName) {}

		virtual ~PekanGUIWindow() = default;

		void render() override final;

	private: /* functions*/

		virtual void _render() = 0;
	};

} // namespace Pekan