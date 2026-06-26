#pragma once

#include <cstdint>
#include <limits>

namespace Pekan
{

	// Stable, persistent identifier for an entity in a Scene.
	//
	// Distinct from entt::entity, which is an internal handle that:
	//   - is recycled after destruction,
	//   - does not survive save/load,
	//   - is meaningless outside of its registry.
	//
	// EntityID values, in contrast, persist in scene files and are used for
	// cross-entity references (e.g. parent-child relationships).
	//
	// Always use EntityID (not uint32_t directly) at call sites,
	// so that the underlying type can change later without breaking them.
	using EntityID = uint32_t;

	// An EntityID value reserved to mean "no entity".
	constexpr EntityID INVALID_ENTITY_ID = 0;

	// The minimum value an EntityID can take.
	constexpr EntityID MIN_ENTITY_ID = 1;

	// The maximum value an EntityID can take.
	constexpr EntityID MAX_ENTITY_ID = std::numeric_limits<EntityID>::max();

} // namespace Pekan
