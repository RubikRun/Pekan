#pragma once

#include "Entity/EntityID.h"

#include <glm/glm.hpp>
#include <json.hpp>

#include <cstdint>

namespace Pekan
{
namespace SerializationUtils
{

	/// Deserializes a 64-bit unsigned integer value from a given JSON field.
	/// Returns true on success, false on failure.
	/// On failure, `result` is left unchanged.
	bool deserializeUint64(const nlohmann::ordered_json& field, uint64_t& result);

	/// Deserializes an EntityID value from a given JSON field.
	/// Returns true on success, false on failure.
	/// On failure, `result` is left unchanged.
	bool deserializeEntityID(const nlohmann::ordered_json& field, EntityID& result);

} // namespace SerializationUtils
} // namespace Pekan

namespace nlohmann
{

	// Defines how a 2D GLM vector is represented in JSON.
	template<>
	struct adl_serializer<glm::vec2>
	{
		static void to_json(ordered_json& jsonData, const glm::vec2& vector)
		{
			jsonData = { vector.x, vector.y };
		}

		static void from_json(const ordered_json& jsonData, glm::vec2& vector)
		{
			jsonData.at(0).get_to(vector.x);
			jsonData.at(1).get_to(vector.y);
		}
	};

	// Defines how a 4D GLM vector is represented in JSON.
	template<>
	struct adl_serializer<glm::vec4>
	{
		static void to_json(ordered_json& jsonData, const glm::vec4& vector)
		{
			jsonData = { vector.x, vector.y, vector.z, vector.w };
		}

		static void from_json(const ordered_json& jsonData, glm::vec4& vector)
		{
			jsonData.at(0).get_to(vector.x);
			jsonData.at(1).get_to(vector.y);
			jsonData.at(2).get_to(vector.z);
			jsonData.at(3).get_to(vector.w);
		}
	};

} // namespace nlohmann
