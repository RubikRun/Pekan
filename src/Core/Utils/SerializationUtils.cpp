#include "SerializationUtils.h"

#include "Entity/EntityID.h"

using json = nlohmann::ordered_json;

namespace Pekan
{
namespace SerializationUtils
{

	/// Checks if a given raw uint64_t entity ID is valid.
	/// A valid entity ID can be safely casted from uint64_t to EntityID.
	static bool isValidEntityId(uint64_t id)
	{
		return id >= MIN_ENTITY_ID && id <= MAX_ENTITY_ID;
	}

//////////
//////////
//////////

	bool deserializeUint64(const json& field, uint64_t& result)
	{
		if (!field.is_number_integer())
		{
			return false;
		}
		// The JSON library stores integers internally as either signed or unsigned values.
		// A signed value is not necessarily negative, so check its actual value.
		if (!field.is_number_unsigned() && field.get<int64_t>() < 0)
		{
			return false;
		}
		result = field.get<uint64_t>();
		return true;
	}

	bool deserializeEntityID(const json& field, EntityID& result)
	{
		uint64_t rawId = 0;
		if (!deserializeUint64(field, rawId))
		{
			return false;
		}
		// Validate the ID before converting it to EntityID,
		// as the conversion could silently truncate an out-of-range value.
		if (!isValidEntityId(rawId))
		{
			return false;
		}
		result = static_cast<EntityID>(rawId);
		return true;
	}

} // namespace SerializationUtils
} // namespace Pekan
