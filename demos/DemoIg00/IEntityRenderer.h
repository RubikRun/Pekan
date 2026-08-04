#pragma once

#include "EntityVisualState.h"

namespace Demo
{

	// Renders an entity from its visual state.
	// Swap implementations to change how sprites look without touching entity logic.
	class IEntityRenderer
	{
	public:
		virtual ~IEntityRenderer() = default;

		virtual void render(const EntityVisualState& state) const = 0;
		virtual void destroy() = 0;
	};

} // namespace Demo
