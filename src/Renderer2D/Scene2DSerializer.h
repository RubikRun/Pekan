#pragma once

#include "SceneSerializer.h"

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
		// TODO: explain what specific things this implementation does
		nlohmann::ordered_json serializeComponents(entt::entity entity, const entt::registry& registry) const override;

		// Deserializes a given components JSON object and emplaces the resulting components on the given entity.
		// TODO: explain what specific things this implementation does
		bool deserializeComponents(const nlohmann::ordered_json& componentsJson, entt::entity entity, entt::registry& registry) const override;

		// Called after deserialization is complete by the base SceneSerializer class
		// TODO: explain what specific things this implementation does
		void postDeserialize(Scene& scene) const override;
	};

} // namespace Renderer2D
} // namespace Pekan
