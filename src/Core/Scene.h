#pragma once

#include "Layer.h"

namespace Pekan
{

	class Scene : public Layer
	{
	public:
		Scene(PekanApplication* application) : Layer(application) {}

		// Can be implemented by derived classes
		// if they want a non-default name for their scene's layer
		virtual std::string getLayerName() const override { return "scene_layer"; }
	};

} // namespace Pekan