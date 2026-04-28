#pragma once

#include "EntityID.h"

namespace Pekan
{

	// A component that stores an identifier for an entity.
	//
	// Attached automatically to every entity created via Scene.
	// Unlike entt::entity, the value in EntityIDComponent survives save/load
	// and can be used for cross-entity references in scene files.
	struct EntityIDComponent
	{
		EntityID id = INVALID_ENTITY_ID;
	};

} // namespace Pekan
