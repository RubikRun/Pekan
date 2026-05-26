#include "SceneSerializer.h"

#include "Scene.h"
#include "Entity/EntityIDComponent.h"
#include "Entity/NameComponent.h"
#include "Entity/DisabledComponent.h"
#include "PekanLogger.h"

using json = nlohmann::ordered_json;

namespace Pekan
{

	std::string SceneSerializer::serialize(const Scene& scene) const
	{
		const entt::registry& registry = scene.getRegistry();

		// The root of the JSON object. Will contain all scene data, serialized.
		json sceneData;

		// Write format version metadata.
		sceneData["formatVersion"] =
		{
			{"major", FORMAT_VERSION_MAJOR},
			{"minor", FORMAT_VERSION_MINOR}
		};

		// Write scene type metadata.
		sceneData["sceneType"] = getSceneType();

		// Write settings object, empty for now.
		sceneData["settings"] = json::object();

		// Write the entities array.
		sceneData["entities"] = serializeEntities(scene, registry);

		// Return the scene data as a JSON string,
		// formatted with 4 spaces for readability.
		return sceneData.dump(4);
	}

	json SceneSerializer::serializeEntities(const Scene& scene, const entt::registry& registry) const
	{
		json entitiesData = json::array();

		// Iterate over all entities in the scene.
		for (entt::entity entity : scene.getEntities())
		{
			PK_ASSERT_QUICK(registry.all_of<EntityIDComponent>(entity));
			const EntityID id = registry.get<EntityIDComponent>(entity).id;
			const NameComponent* nameComponent = registry.try_get<NameComponent>(entity);

			// The JSON object for this entity.
			json entityData;

			// Write entity ID.
			entityData["id"] = id;

			// Write entity name, if NameComponent is present and non-empty.
			// Otherwise it's okay to omit the field, as entity names are optional.
			if (nameComponent != nullptr && !nameComponent->name.empty())
			{
				entityData["name"] = nameComponent->name;
			}

			// Write enabled status, if DisabledComponent is present.
			// Otherwise it's okay to omit the field, as entities are enabled by default.
			const bool isEnabled = !registry.all_of<DisabledComponent>(entity);
			if (!isEnabled)
			{
				entityData["enabled"] = false;
			}

			// Write components sub-object,
			// whose creation is delegated to the scene-type-specific serializer.
			entityData["components"] = serializeComponents(entity, registry);

			// Entity data object is now complete.
			// Add it to the entities array.
			entitiesData.push_back(std::move(entityData));
		}

		return entitiesData;
	}

	bool SceneSerializer::deserialize(const std::string& jsonText, Scene& scene) const
	{
		// TODO: implement
		return false;
	}

} // namespace Pekan
