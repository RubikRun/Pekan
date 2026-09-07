#pragma once

#include "SceneSerializer.h"

#include <json.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A class for (de)serializing a Scene2D to/from a .pksc file.
	class Scene2DSerializer : public SceneSerializer
	{
private: /* functions */

		std::string getSceneType() const override { return "scene2d"; }

		// Serializes all Scene2D components on the given entity.
		nlohmann::ordered_json serializeComponents(entt::entity entity, const entt::registry& registry) const override;

		// Deserializes all Scene2D components from the given components JSON object
		// and emplaces the resulting components on the given entity
		bool deserializeComponents
		(
			const nlohmann::ordered_json& componentsJson,
			entt::entity entity,
			entt::registry& registry,
			const std::unordered_map<std::string, EntityID>& entityNameToIdMap
		) const override;

		// Called after deserialization is complete by the base SceneSerializer class
		// TODO: explain what specific things this implementation does
		void postDeserialize(Scene& scene) const override;
	};

} // namespace Renderer2D
} // namespace Pekan
