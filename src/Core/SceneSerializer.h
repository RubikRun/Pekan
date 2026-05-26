#pragma once

#include <entt/entt.hpp>
#include <json.hpp>

#include <string>

namespace Pekan
{

	class Scene;

	// Base class for (de)serializing a Scene to/from a .pksc file.
	//
	// Handles everything that is common to all scene types:
	//   - reading/writing the top-level JSON structure
	//     (formatVersion, sceneType, settings, entities)
	//   - creating entities and emplacing EntityIDComponent, NameComponent, DisabledComponent
	//   - delegating component (de)serialization to virtual hooks.
	//
	// Subclasses are responsible for handling the components specific to their scene type
	// (e.g. Scene2DSerializer in Renderer2D handles all components valid for sceneType = "scene2d").
	class SceneSerializer
	{
	public: /* functions */

		virtual ~SceneSerializer() = default;

		// Serializes the given Scene to a JSON string in the .pksc format.
		std::string serialize(const Scene& scene) const;

		// Deserializes a Scene from a .pksc JSON string (content of a .pksc file),
		// populating the given (typically empty) Scene with its contents.
		//
		// Returns true on success, false on any fatal error. Fatal errors include:
		//   - malformed JSON
		//   - missing/unknown sceneType
		//   - file's formatVersion.major higher than the supported major version
		//   - duplicate EntityIDs
		//   - invalid parent name references (not found / ambiguous due to duplicates)
		// Non-fatal issues (unknown component keys, higher minor version, ...)
		// are reported as warnings and the load continues.
		//
		// On failure the state of `scene` is unspecified.
		// Callers wanting transactional behavior should deserialize into a staging Scene
		// and only adopt it on success.
		bool deserialize(const std::string& jsonText, Scene& scene) const;

	protected: /* functions */

		// Returns the value that's supposed to be in the JSON's "sceneType" field.
		// For example, Scene2DSerializer returns "scene2d".
		virtual std::string getSceneType() const = 0;

		// Serializes all components on the given entity that are relevant to this scene type.
		//
		// The returned JSON value must be an object whose keys are component type names
		// (e.g. "Transform2D", "RectangleGeometry", ...) and whose values are component data.
		virtual nlohmann::ordered_json serializeComponents(entt::entity entity, const entt::registry& registry) const = 0;

		// Deserializes a given components JSON object
		// and emplaces the resulting components on the given entity.
		//
		// `componentsJson` is the value of the entity's "components" field — an object whose
		// keys are component type names. Unknown keys will be reported as warnings and ignored.
		virtual void deserializeComponents(const nlohmann::ordered_json& componentsJson, entt::entity entity, entt::registry& registry) const = 0;

		// Optional hook called after deserialization is complete (every entity has been created and its components emplaced).
		//
		// Used, for example, to resolve cross-entity references (e.g. Transform2D parent EntityIDs -> entt::entity handles).
		virtual void postDeserialize(Scene& scene) const {}

	protected: /* constants */

		// Format version supported by this serializer.
		// - Reading a file with a higher major version than this is refused.
		// - Reading a file with a higher minor version than this is allowed (with a warning).
		// - Writing always produces a file with this version.
		static constexpr int FORMAT_VERSION_MAJOR = 1;
		static constexpr int FORMAT_VERSION_MINOR = 0;

	private: /* functions */

		// Builds the JSON array for the scene's entities.
		nlohmann::ordered_json serializeEntities(const Scene& scene, const entt::registry& registry) const;
	};

} // namespace Pekan
