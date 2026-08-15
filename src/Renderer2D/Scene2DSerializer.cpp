#include "Scene2DSerializer.h"

using json = nlohmann::ordered_json;

namespace Pekan
{
namespace Renderer2D {

	json Scene2DSerializer::serializeComponents(entt::entity entity, const entt::registry& registry) const
	{
		// TODO: implement
		return {};
	}

	// Deserializes a given components JSON object and emplaces the resulting components on the given entity.
	// TODO: explain what specific things this implementation does
	bool Scene2DSerializer::deserializeComponents(const json& componentsJson, entt::entity entity, entt::registry& registry) const
	{
		// TODO: implement
		return false;
	}

	// Called after deserialization is complete by the base SceneSerializer class
	// TODO: explain what specific things this implementation does
	void Scene2DSerializer::postDeserialize(Scene& scene) const
	{
		// TODO: implement
	}

} // namespace Renderer2D
} // namespace Pekan
