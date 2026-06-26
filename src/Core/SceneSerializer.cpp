#include "SceneSerializer.h"

#include "Scene.h"
#include "Entity/EntityIDComponent.h"
#include "Entity/NameComponent.h"
#include "Entity/DisabledComponent.h"
#include "PekanLogger.h"

#include <cstdint>

using json = nlohmann::ordered_json;

namespace Pekan
{

	/// A struct holding the values of the top-level JSON fields of an entity.
	struct EntityTopLevelData
	{
		EntityID id = INVALID_ENTITY_ID;
		std::string name = "";
		bool enabled = true;
		const json* componentsData = nullptr;
	};

	/// Deserializes the format version of a given scene JSON object.
	/// Returns true on success, false on error.
	static bool deserializeFormatVersion(const json& sceneData, FormatVersion& formatVersion)
	{
		// Get the format version.
		const auto itFormatVersion = sceneData.find("formatVersion");
		if (itFormatVersion == sceneData.end())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file is missing the \"formatVersion\" field.", "Pekan");
			return false;
		}
		if (!itFormatVersion->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"formatVersion\" field is not an object.", "Pekan");
			return false;
		}

		// Get the major and minor version numbers.
		const auto itMajor = itFormatVersion->find("major");
		if (itMajor == itFormatVersion->end())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"formatVersion\" field is missing the \"major\" field.", "Pekan");
			return false;
		}
		const auto itMinor = itFormatVersion->find("minor");
		if (itMinor == itFormatVersion->end())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"formatVersion\" field is missing the \"minor\" field.", "Pekan");
			return false;
		}
		if (!itMajor->is_number_integer())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"formatVersion\" field's \"major\" field is not an integer.", "Pekan");
			return false;
		}
		if (!itMinor->is_number_integer())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"formatVersion\" field's \"minor\" field is not an integer.", "Pekan");
			return false;
		}

		const int versionMajor = itMajor->get<int>();
		const int versionMinor = itMinor->get<int>();
		formatVersion = { versionMajor, versionMinor };
		return true;
	}

	/// Validates the format version of a given scene JSON object.
	/// Returns true on valid format version, false on invalid/error.
	static bool validateFormatVersion(const json& sceneData, FormatVersion supportedVersion)
	{
		// Deserialize format version.
		FormatVersion formatVersion;
		if (!deserializeFormatVersion(sceneData, formatVersion))
		{
			return false;
		}
		// If major version is higher than supported, refuse to load.
		if (formatVersion.versionMajor > supportedVersion.versionMajor)
		{
			PK_LOG_ERROR("Cannot load a scene file with format version "
				<< formatVersion.versionMajor << "." << formatVersion.versionMinor
				<< " because its major version is higher than that of the supported format version "
				<< supportedVersion.versionMajor << "." << supportedVersion.versionMinor << ".", "Pekan");
			return false;
		}
		// If major version is the same as supported, but minor version is higher, log a warning and continue loading.
		if (formatVersion.versionMajor == supportedVersion.versionMajor && formatVersion.versionMinor > supportedVersion.versionMinor)
		{
			PK_LOG_WARNING("Loading a scene file with format version "
				<< formatVersion.versionMajor << "." << formatVersion.versionMinor
				<< " whose minor version is higher than that of the supported format version "
				<< supportedVersion.versionMajor << "." << supportedVersion.versionMinor
				<< ". Unknown components/fields will be ignored.", "Pekan");
		}

		return true;
	}

	/// Deserializes the scene type of a given scene JSON object.
	/// Returns true on success, false on error.
	static bool deserializeSceneType(const json& sceneData, std::string& sceneType)
	{
		// Get the scene type.
		const auto itSceneType = sceneData.find("sceneType");
		if (itSceneType == sceneData.end())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file is missing the \"sceneType\" field.", "Pekan");
			return false;
		}
		if (!itSceneType->is_string())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"sceneType\" field is not a string.", "Pekan");
			return false;
		}

		sceneType = itSceneType->get<std::string>();
		return true;
	}

	/// Validates the scene type of a given scene JSON object.
	/// Returns true on valid scene type, false on invalid/error.
	static bool validateSceneType(const json& sceneData, const std::string& supportedSceneType)
	{
		// Deserialize scene type.
		std::string sceneType;
		if (!deserializeSceneType(sceneData, sceneType))
		{
			return false;
		}
		if (sceneType != supportedSceneType)
		{
			PK_LOG_ERROR("Cannot load a scene of type \"" << sceneType
				<< "\". This serializer supports only scenes of type \"" << supportedSceneType << "\".", "Pekan");
			return false;
		}

		return true;
	}

	/// Deserializes the ID of a given entity JSON object, without validating it.
	/// The ID is deserialized as a raw uint64_t so that out-of-range values survive for the caller to validate.
	/// An ID is inherently unsigned, so negative values are discarded here, as part of deserialization.
	/// Returns true on success, false on error.
	static bool deserializeEntityId(const json& entityData, uint64_t& id)
	{
		// Get entity's ID.
		const auto itId = entityData.find("id");
		if (itId == entityData.end())
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. Entity object is missing the \"id\" field.", "Pekan");
			return false;
		}
		if (!itId->is_number_integer())
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. Entity object's \"id\" field is not an integer.", "Pekan");
			return false;
		}

		// The JSON library stores integers internally as either signed or unsigned values.
		// A signed value is not necessarily negative, so check its actual value.
		if (!itId->is_number_unsigned() && itId->get<int64_t>() < 0)
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. Entity's ID (" << itId->get<int64_t>()
				<< ") cannot be negative.", "Pekan");
			return false;
		}

		id = itId->get<uint64_t>();
		return true;
	}

	/// Validates a raw deserialized entity ID.
	/// Returns true on valid ID, false on invalid.
	static bool validateEntityId(uint64_t id)
	{
		if (id < MIN_ENTITY_ID)
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. Entity's ID (" << id
				<< ") must be at least " << MIN_ENTITY_ID << ".", "Pekan");
			return false;
		}
		if (id > MAX_ENTITY_ID)
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. Entity's ID (" << id
				<< ") can be at most " << MAX_ENTITY_ID << ".", "Pekan");
			return false;
		}

		return true;
	}

	/// Deserializes the name of a given entity JSON object.
	/// `id` is the entity's already deserialized ID, used only to identify the entity in error messages.
	/// Returns:
	/// - true, on success (name retrieved successfully or name missing, both are valid)
	/// - false, on error (name exists but is invalid, e.g. a number)
	static bool deserializeEntityName(const json& entityData, EntityID id, std::string& name)
	{
		// Get entity's name.
		const auto itName = entityData.find("name");
		if (itName == entityData.end())
		{
			// A missing name is valid.
			return true;
		}
		if (!itName->is_string())
		{
			PK_LOG_ERROR("Failed to deserialize entity with ID " << id << " from a scene file. Entity object's \"name\" field is not a string.", "Pekan");
			return false;
		}

		name = itName->get<std::string>();
		return true;
	}

	/// Deserializes the enabled status of a given entity JSON object.
	/// `id` is the entity's already deserialized ID, used only to identify the entity in error messages.
	/// Returns true on success, false on error.
	static bool deserializeEntityEnabled(const json& entityData, EntityID id, bool& enabled)
	{
		// Get entity's enabled status
		const auto itEnabled = entityData.find("enabled");
		if (itEnabled == entityData.end())
		{
			// A missing enabled status is valid.
			return true;
		}
		if (!itEnabled->is_boolean())
		{
			PK_LOG_ERROR("Failed to deserialize entity with ID " << id << " from a scene file. Entity object's \"enabled\" field is not a boolean.", "Pekan");
			return false;
		}

		enabled = itEnabled->get<bool>();
		return true;
	}

	/// Retrieves the components data (a JSON object itself) of a given entity JSON object.
	/// `id` is the entity's already-deserialized ID, used only to identify the entity in error messages.
	/// Returns true on success, false on error.
	static bool getEntityComponentsData(const json& entityData, EntityID id, const json*& componentsData)
	{
		// Get entity's components object
		const auto itComponents = entityData.find("components");
		if (itComponents == entityData.end())
		{
			PK_LOG_ERROR("Failed to deserialize entity with ID " << id << " from a scene file. Entity object is missing the \"components\" field.", "Pekan");
			return false;
		}
		if (!itComponents->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize entity with ID " << id << " from a scene file. Entity object's \"components\" field is not an object.", "Pekan");
			return false;
		}

		componentsData = &(*itComponents);
		return true;
	}

	/// Deserializes the top-level data of a given entity JSON object.
	/// Returns true on success, false on error.
	static bool deserializeEntityTopLevelData(const json& entityData, EntityTopLevelData& entityTopLevelData)
	{
		if (!entityData.is_object())
		{
			PK_LOG_ERROR("Failed to deserialize an entity from a scene file. The entity is not an object.", "Pekan");
			return false;
		}

		// Deserialize ID
		uint64_t rawId = 0;
		if (!deserializeEntityId(entityData, rawId))
		{
			return false;
		}
		// Validate the ID before converting it to EntityID,
		// as the conversion could silently truncate an out-of-range value.
		if (!validateEntityId(rawId))
		{
			return false;
		}
		entityTopLevelData.id = static_cast<EntityID>(rawId);
		// Deserialize name
		if (!deserializeEntityName(entityData, entityTopLevelData.id, entityTopLevelData.name))
		{
			return false;
		}
		// Deserialize enabled status
		if (!deserializeEntityEnabled(entityData, entityTopLevelData.id, entityTopLevelData.enabled))
		{
			return false;
		}
		// Get components data
		if (!getEntityComponentsData(entityData, entityTopLevelData.id, entityTopLevelData.componentsData))
		{
			return false;
		}

		return true;
	}

//////////
//////////
//////////

	std::string SceneSerializer::serialize(const Scene& scene) const
	{
		const entt::registry& registry = scene.getRegistry();

		// The root of the JSON object. Will contain all scene data, serialized.
		json sceneData;

		// Write format version metadata.
		sceneData["formatVersion"] =
		{
			{"major", FORMAT_VERSION_SUPPORTED.versionMajor},
			{"minor", FORMAT_VERSION_SUPPORTED.versionMinor}
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

	bool SceneSerializer::deserialize(const std::string& jsonText, Scene& scene) const
	{
		try
		{
			// Parse the given JSON text into a JSON object containing all scene data.
			json sceneData = json::parse(jsonText);

			// Check if scene's format version is supported.
			if (!validateFormatVersion(sceneData, FORMAT_VERSION_SUPPORTED))
			{
				return false;
			}
			// Check if scene's type is supported by this serializer.
			if (!validateSceneType(sceneData, getSceneType()))
			{
				return false;
			}

			//////////
			// NOTE: We can deserialize settings here, but we don't support any settings yet,
			//       and the "settings" field is optional, so there's nothing to do yet.
			//////////

			// Deserialize all entities
			if (!deserializeEntities(sceneData, scene))
			{
				return false;
			}

			// Perform post-deserialization operations.
			postDeserialize(scene);

			return true;
		}
		catch (const json::parse_error& e)
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file contains invalid JSON syntax (JSON parse error: "
				<< e.what() << ").", "Pekan");
		}
		catch (const json::exception& e)
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file contains invalid data (JSON error: "
				<< e.what() << ").", "Pekan");
		}

		return false;
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

	bool SceneSerializer::deserializeEntities(const json& sceneData, Scene& scene) const
	{
		// Get the entities array.
		const auto itEntities = sceneData.find("entities");
		if (itEntities == sceneData.end())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file is missing the \"entities\" field.", "Pekan");
			return false;
		}
		if (!itEntities->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file's \"entities\" field is not an array.", "Pekan");
			return false;
		}

		entt::registry& registry = scene.getRegistry();

		// A set of all entity IDs loaded so far.
		std::unordered_set<EntityID> entityIds;

		// Deserialize each entity from the array into the scene
		for (const auto& entityData : *itEntities)
		{
			// Deserialize the entity from the JSON object into the scene.
			if (!deserializeEntity(entityData, scene, registry, entityIds))
			{
				return false;
			}
		}

		// Sync scene's "next EntityID" counter to the loaded entities.
		// If the counter cannot be synced, the scene could still be loaded successfully,
		// and it would be fine until a new entity is created in the scene,
		// that's when it would fail. That would be a midleading failure point
		// and tricky to debug, so it's better to refuse to load such scenes right away.
		if (!scene.syncNextEntityIdToEntities())
		{
			PK_LOG_ERROR("Failed to deserialize a scene, because we couldn't sync scene's \"next EntityID\" counter, "
				"which would leave the scene unable to generate IDs for new entities.", "Pekan");
			return false;
		}

		return true;
	}

	bool SceneSerializer::deserializeEntity(const json& entityData, Scene& scene, entt::registry& registry, std::unordered_set<EntityID>& entityIds) const
	{
		EntityTopLevelData entityTopLevelData;
		if (!deserializeEntityTopLevelData(entityData, entityTopLevelData))
		{
			return false;
		}
		const EntityID entityId = entityTopLevelData.id;
		PK_ASSERT_QUICK(entityId != INVALID_ENTITY_ID);

		// Don't allow duplicate IDs.
		if (entityIds.contains(entityId))
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file contains more than one entity with ID " << entityId << ".", "Pekan");
			return false;
		}

		// Create an entity in the scene
		const entt::entity entity = scene.createEntity(entityTopLevelData.id);
		entityIds.insert(entityId);
		// If entity's deserialized name is not empty, emplace a NameComponent on the entity
		if (!entityTopLevelData.name.empty())
		{
			registry.emplace<NameComponent>(entity, entityTopLevelData.name);
		}
		// If entity's deserialized enabled status is false, disable the entity
		if (!entityTopLevelData.enabled)
		{
			scene.disableEntity(entity);
		}
		// Deserialize entity's components
		if (!deserializeComponents(*entityTopLevelData.componentsData, entity, registry))
		{
			PK_LOG_ERROR("Failed to deserialize the components of entity with ID " << entityId << " from a scene file.", "Pekan");
			return false;
		}

		return true;
	}

} // namespace Pekan
