#include "Scene2DSerializer.h"

#include "Entity/EntityID.h"
#include "PekanLogger.h"
#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "LineGeometryComponent.h"
#include "SolidColorMaterialComponent.h"
#include "LineComponent.h"
#include "CameraComponent2D.h"

#include "Scene.h"
#include "Entity/EntityIDComponent.h"
#include "Utils/SerializationUtils.h" // IWYU pragma: keep

#include <initializer_list>

using json = nlohmann::ordered_json;

namespace Pekan
{
namespace Renderer2D
{

namespace {
	// A temporary internal ECS component used to carry an unresolved ID of a TransformComponent2D's parent.
	// "Unresolved" here means that its an EntityID instead of an entt::entity handle. A parent's EntityID can be
	// matched to the corresponding entt::entity handle once the parent actually exists as an entity in Pekan's ECS.
	// Until then the parent stays unresolved and is saved in this temporary component.
	//
	// This component can be placed on an entity during deserializeComponents()
	// and will be later resolved in postDeserialize(). After postDeserialize() has run, this component will be
	// removed from the entity, and TransformComponent2D's parent will be set to the actual entt::entity handle.
	struct UnresolvedTransformParentComponent2D_
	{
		EntityID parentId = INVALID_ENTITY_ID;
	};
}

	// Serializes a given transform component into a JSON object.
	static json serializeTransformComponent(const TransformComponent2D& transformComponent, const entt::registry& registry)
	{
		json transformData =
		{
			{ "position", transformComponent.position },
			{ "rotation", transformComponent.rotation },
			{ "scaleFactor", transformComponent.scaleFactor }
		};
		if (transformComponent.parent != entt::null)
		{
			const EntityID parentId = registry.get<EntityIDComponent>(transformComponent.parent).id;
			transformData["parent"] = parentId;
		}
		return transformData;
	}

	// Serializes a given sprite component into a JSON object.
	static json serializeSpriteComponent(const SpriteComponent& spriteComponent)
	{
		const json spriteData =
		{
			{ "width", spriteComponent.width },
			{ "height", spriteComponent.height },
			// NOTE: There is no way to evaluate the correct texture path yet because we don't store the source path in Texture2D.
			//       For now we'll always set texturePath to null explicitly, instead of just skipping it,
			//       to remember later that it needs to be implemented.
			{ "texturePath", nullptr },
			{ "textureCoordinatesMin", spriteComponent.textureCoordinatesMin },
			{ "textureCoordinatesMax", spriteComponent.textureCoordinatesMax }
		};
		return spriteData;
	}

	// Serializes a given rectangle geometry component into a JSON object.
	static json serializeRectangleGeometryComponent(const RectangleGeometryComponent& rectangleGeometryComponent)
	{
		const json rectangleData =
		{
			{ "width", rectangleGeometryComponent.width },
			{ "height", rectangleGeometryComponent.height }
		};
		return rectangleData;
	}

	// Serializes a given circle geometry component into a JSON object.
	static json serializeCircleGeometryComponent(const CircleGeometryComponent& circleGeometryComponent)
	{
		const json circleData =
		{
			{ "radius", circleGeometryComponent.radius },
			{ "segmentsCount", circleGeometryComponent.segmentsCount }
		};
		return circleData;
	}

	// Serializes a given triangle geometry component into a JSON object.
	static json serializeTriangleGeometryComponent(const TriangleGeometryComponent& triangleGeometryComponent)
	{
		const json triangleData =
		{
			{ "pointA", triangleGeometryComponent.pointA },
			{ "pointB", triangleGeometryComponent.pointB },
			{ "pointC", triangleGeometryComponent.pointC }
		};
		return triangleData;
	}

	// Serializes a given polygon geometry component into a JSON object.
	static json serializePolygonGeometryComponent(const PolygonGeometryComponent& polygonGeometryComponent)
	{
		const json polygonData =
		{
			{ "vertexPositions", polygonGeometryComponent.vertexPositions }
		};
		return polygonData;
	}

	// Serializes a given line geometry component into a JSON object.
	static json serializeLineGeometryComponent(const LineGeometryComponent& lineGeometryComponent)
	{
		const json lineData =
		{
			{ "pointA", lineGeometryComponent.pointA },
			{ "pointB", lineGeometryComponent.pointB },
			{ "thickness", lineGeometryComponent.thickness }
		};
		return lineData;
	}

	// Serializes a given solid color material component into a JSON object.
	static json serializeSolidColorMaterialComponent(const SolidColorMaterialComponent& solidColorMaterialComponent)
	{
		const json solidColorMaterialData =
		{
			{ "color", solidColorMaterialComponent.color }
		};
		return solidColorMaterialData;
	}

	// Serializes a given line component into a JSON object.
	static json serializeLineComponent(const LineComponent& lineComponent)
	{
		const json lineData =
		{
			{ "pointA", lineComponent.pointA },
			{ "pointB", lineComponent.pointB },
			{ "color", lineComponent.color }
		};
		return lineData;
	}

	// Serializes a given camera component into a JSON object.
	static json serializeCameraComponent2D(const CameraComponent2D& cameraComponent2D)
	{
		const json cameraData =
		{
			{ "size", cameraComponent2D.size },
			{ "position", cameraComponent2D.position },
			{ "rotation", cameraComponent2D.rotation },
			{ "zoomLevel", cameraComponent2D.zoomLevel },
			{ "isPrimary", cameraComponent2D.isPrimary },
			{ "isControllable", cameraComponent2D.isControllable }
		};
		return cameraData;
	}

	// Logs a warning for each unknown field in a given component JSON object.
	// Unknown fields are skipped, they do NOT abort the load.
	static void warnUnknownComponentFields
	(
		const json& componentData,
		const std::string& componentName,
		std::initializer_list<const char*> knownFields
	)
	{
		for (auto it = componentData.begin(); it != componentData.end(); ++it)
		{
			bool isKnownField = false;
			for (const char* knownField : knownFields)
			{
				if (it.key() == knownField)
				{
					isKnownField = true;
					break;
				}
			}

			if (!isKnownField)
			{
				PK_LOG_WARNING("Ignoring unknown field \"" << it.key() << "\" in a " << componentName
					<< " component because it is not valid for that component type.", "Pekan");
			}
		}
	}

	// Deserializes the position of a given transform component JSON.
	static bool deserializeTransformComponentPosition
	(
		const json& transformComponentData,
		float& positionX,
		float& positionY
	)
	{
		const auto itPosition = transformComponentData.find("position");
		if (itPosition == transformComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPosition->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's position because its \"position\" field is not an array.", "Pekan");
			return false;
		}
		if (itPosition->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's position. The \"position\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPosition->at(0).is_number() || !itPosition->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's position. The \"position\" array's elements must be numbers.", "Pekan");
			return false;
		}
		positionX = itPosition->at(0).get<float>();
		positionY = itPosition->at(1).get<float>();
		return true;
	}

	// Deserializes the rotation of a given transform component JSON.
	static bool deserializeTransformComponentRotation
	(
		const json& transformComponentData,
		float& rotation
	)
	{
		const auto itRotation = transformComponentData.find("rotation");
		if (itRotation == transformComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itRotation->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's rotation. The \"rotation\" field must be a number.", "Pekan");
			return false;
		}
		rotation = itRotation->get<float>();
		return true;
	}

	// Deserializes the scale factor of a given transform component JSON.
	static bool deserializeTransformComponentScaleFactor
	(
		const json& transformComponentData,
		float& scaleFactorX,
		float& scaleFactorY
	)
	{
		const auto itScaleFactor = transformComponentData.find("scaleFactor");
		if (itScaleFactor == transformComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itScaleFactor->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's scale factor because its \"scaleFactor\" field is not an array.", "Pekan");
			return false;
		}
		if (itScaleFactor->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's scale factor. The \"scaleFactor\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itScaleFactor->at(0).is_number() || !itScaleFactor->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's scale factor. The \"scaleFactor\" array's elements must be numbers.", "Pekan");
			return false;
		}
		scaleFactorX = itScaleFactor->at(0).get<float>();
		scaleFactorY = itScaleFactor->at(1).get<float>();
		return true;
	}

	// Deserializes the parent of a given transform component JSON.
	static bool deserializeTransformComponentParent
	(
		const json& transformComponentData,
		const std::unordered_map<std::string, EntityID>& entityNameToIdMap,
		EntityID& parentId
	)
	{
		const auto itParent = transformComponentData.find("parent");
		if (itParent == transformComponentData.end() || itParent->is_null())
		{
			// A missing or null parent is valid and means that the entity has no parent.
			parentId = INVALID_ENTITY_ID;
		}
		else if (itParent->is_number_integer())
		{
			// The JSON library stores integers internally as either signed or unsigned values.
			// A signed value is not necessarily negative, so check its actual value.
			if (!itParent->is_number_unsigned() && itParent->get<int64_t>() < 0)
			{
				PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent. Parent's ID (" << itParent->get<int64_t>()
					<< ") cannot be negative.", "Pekan");
				return false;
			}

			// Deserialize as a raw uint64_t so that out-of-range values survive for validation.
			const uint64_t rawParentId = itParent->get<uint64_t>();
			// Validate the ID before converting it to EntityID,
			// as the conversion could silently truncate an out-of-range value.
			if (rawParentId < MIN_ENTITY_ID)
			{
				PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent. Parent's ID (" << rawParentId
					<< ") must be at least " << MIN_ENTITY_ID << ".", "Pekan");
				return false;
			}
			if (rawParentId > MAX_ENTITY_ID)
			{
				PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent. Parent's ID (" << rawParentId
					<< ") can be at most " << MAX_ENTITY_ID << ".", "Pekan");
				return false;
			}

			parentId = static_cast<EntityID>(rawParentId);
		}
		else if (itParent->is_string())
		{
			const std::string parentName = itParent->get<std::string>();
			if (parentName.empty())
			{
				PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent because an empty string is not a valid entity name.", "Pekan");
				return false;
			}

			const auto itParentId = entityNameToIdMap.find(parentName);
			if (itParentId != entityNameToIdMap.end())
			{
				if (itParentId->second == INVALID_ENTITY_ID)
				{
					PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent \"" << parentName << "\""
					             " because that name is ambiguous (multiple entities in the scene have that name).", "Pekan");
					return false;
				}
				// At this point we have a valid parent ID
				parentId = itParentId->second;
			}
			else
			{
				PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent \"" << parentName << "\""
				             " because that name doesn't belong to any entity in the scene.", "Pekan");
				return false;
			}
		}
		else
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component's parent. The \"parent\" field must be "
				         "either an integer (ID of an entity), a string (name of an entity) or null (no parent).", "Pekan");
			return false;
		}

		return true;
	}

	// Enum for different results of deserializing a component.
	enum class DeserializeComponentResult
	{
		FoundAndOk,       // Component was found and deserialized successfully
		FoundAndError,    // Component was found but failed to deserialize.
		NotFound          // Component was NOT found (which might be ok or not depending on the specific type of component)
	};

	// Deserializes the Transform2D component from a given components JSON object, if present.
	// @param[out] parentId - The deserialized parent ID will be written here, if deserialized successfully.
	//                        On failure, the parameter is left untouched.
	static DeserializeComponentResult deserializeTransformComponent
	(
		const json& componentsData,
		const std::unordered_map<std::string, EntityID>& entityNameToIdMap,
		TransformComponent2D& transformComponent,    // output parameter where the deserialized data is filled into
		EntityID& parentId    // output parameter where the deserialized parent ID is written
	)
	{
		// Get the transform component
		const auto itTransformComponent = componentsData.find("Transform2D");
		if (itTransformComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itTransformComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component because the \"Transform2D\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itTransformComponent,
			"Transform2D",
			{ "position", "rotation", "scaleFactor", "parent" }
		);

		// Deserialize the position, rotation, scale factor and parent
		if (
			!deserializeTransformComponentPosition(*itTransformComponent, transformComponent.position.x, transformComponent.position.y) ||
			!deserializeTransformComponentRotation(*itTransformComponent, transformComponent.rotation) ||
			!deserializeTransformComponentScaleFactor(*itTransformComponent, transformComponent.scaleFactor.x, transformComponent.scaleFactor.y) ||
			!deserializeTransformComponentParent(*itTransformComponent, entityNameToIdMap, parentId)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a Transform2D component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the width of a given sprite component JSON.
	static bool deserializeSpriteComponentWidth
	(
		const json& spriteComponentData,
		float& width
	)
	{
		const auto itWidth = spriteComponentData.find("width");
		if (itWidth == spriteComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itWidth->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's width. The \"width\" field must be a number.", "Pekan");
			return false;
		}
		width = itWidth->get<float>();
		return true;
	}

	// Deserializes the height of a given sprite component JSON.
	static bool deserializeSpriteComponentHeight
	(
		const json& spriteComponentData,
		float& height
	)
	{
		const auto itHeight = spriteComponentData.find("height");
		if (itHeight == spriteComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itHeight->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's height. The \"height\" field must be a number.", "Pekan");
			return false;
		}
		height = itHeight->get<float>();
		return true;
	}

	// Deserializes the texture of a given sprite component JSON.
	// NOTE: Loading a texture from a path is not implemented yet.
	//       For now we only accept null. A non-null string is ignored with a warning.
	static bool deserializeSpriteComponentTexture
	(
		const json& spriteComponentData
	)
	{
		const auto itTexturePath = spriteComponentData.find("texturePath");
		if (itTexturePath == spriteComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (itTexturePath->is_null())
		{
			return true;
		}
		if (!itTexturePath->is_string())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's texture. The \"texturePath\" field must be a string or null.", "Pekan");
			return false;
		}
		PK_LOG_WARNING("Ignoring a Sprite component's texture path \"" << itTexturePath->get<std::string>()
			<< "\" because loading textures from a path is not yet implemented.", "Pekan");
		return true;
	}

	// Deserializes the min texture coordinates of a given sprite component JSON.
	static bool deserializeSpriteComponentTextureCoordinatesMin
	(
		const json& spriteComponentData,
		float& textureCoordinatesMinX,
		float& textureCoordinatesMinY
	)
	{
		const auto itTextureCoordinatesMin = spriteComponentData.find("textureCoordinatesMin");
		if (itTextureCoordinatesMin == spriteComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itTextureCoordinatesMin->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's min texture coordinates because its \"textureCoordinatesMin\" field is not an array.", "Pekan");
			return false;
		}
		if (itTextureCoordinatesMin->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's min texture coordinates. The \"textureCoordinatesMin\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itTextureCoordinatesMin->at(0).is_number() || !itTextureCoordinatesMin->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's min texture coordinates. The \"textureCoordinatesMin\" array's elements must be numbers.", "Pekan");
			return false;
		}
		textureCoordinatesMinX = itTextureCoordinatesMin->at(0).get<float>();
		textureCoordinatesMinY = itTextureCoordinatesMin->at(1).get<float>();
		return true;
	}

	// Deserializes the max texture coordinates of a given sprite component JSON.
	static bool deserializeSpriteComponentTextureCoordinatesMax
	(
		const json& spriteComponentData,
		float& textureCoordinatesMaxX,
		float& textureCoordinatesMaxY
	)
	{
		const auto itTextureCoordinatesMax = spriteComponentData.find("textureCoordinatesMax");
		if (itTextureCoordinatesMax == spriteComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itTextureCoordinatesMax->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's max texture coordinates because its \"textureCoordinatesMax\" field is not an array.", "Pekan");
			return false;
		}
		if (itTextureCoordinatesMax->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's max texture coordinates. The \"textureCoordinatesMax\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itTextureCoordinatesMax->at(0).is_number() || !itTextureCoordinatesMax->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component's max texture coordinates. The \"textureCoordinatesMax\" array's elements must be numbers.", "Pekan");
			return false;
		}
		textureCoordinatesMaxX = itTextureCoordinatesMax->at(0).get<float>();
		textureCoordinatesMaxY = itTextureCoordinatesMax->at(1).get<float>();
		return true;
	}

	// Deserializes the Sprite component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeSpriteComponent
	(
		const json& componentsData,
		SpriteComponent& spriteComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the sprite component
		const auto itSpriteComponent = componentsData.find("Sprite");
		if (itSpriteComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itSpriteComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component because the \"Sprite\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itSpriteComponent,
			"Sprite",
			{ "width", "height", "texturePath", "textureCoordinatesMin", "textureCoordinatesMax" }
		);

		// Deserialize the width, height, texture path and texture coordinates
		if (
			!deserializeSpriteComponentWidth(*itSpriteComponent, spriteComponent.width) ||
			!deserializeSpriteComponentHeight(*itSpriteComponent, spriteComponent.height) ||
			!deserializeSpriteComponentTexture(*itSpriteComponent) ||
			!deserializeSpriteComponentTextureCoordinatesMin(*itSpriteComponent, spriteComponent.textureCoordinatesMin.x, spriteComponent.textureCoordinatesMin.y) ||
			!deserializeSpriteComponentTextureCoordinatesMax(*itSpriteComponent, spriteComponent.textureCoordinatesMax.x, spriteComponent.textureCoordinatesMax.y)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a Sprite component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the width of a given rectangle geometry component JSON.
	static bool deserializeRectangleGeometryComponentWidth
	(
		const json& rectangleGeometryComponentData,
		float& width
	)
	{
		const auto itWidth = rectangleGeometryComponentData.find("width");
		if (itWidth == rectangleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itWidth->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a RectangleGeometry component's width. The \"width\" field must be a number.", "Pekan");
			return false;
		}
		width = itWidth->get<float>();
		return true;
	}

	// Deserializes the height of a given rectangle geometry component JSON.
	static bool deserializeRectangleGeometryComponentHeight
	(
		const json& rectangleGeometryComponentData,
		float& height
	)
	{
		const auto itHeight = rectangleGeometryComponentData.find("height");
		if (itHeight == rectangleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itHeight->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a RectangleGeometry component's height. The \"height\" field must be a number.", "Pekan");
			return false;
		}
		height = itHeight->get<float>();
		return true;
	}

	// Deserializes the RectangleGeometry component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeRectangleGeometryComponent
	(
		const json& componentsData,
		RectangleGeometryComponent& rectangleGeometryComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the rectangle geometry component
		const auto itRectangleGeometryComponent = componentsData.find("RectangleGeometry");
		if (itRectangleGeometryComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itRectangleGeometryComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a RectangleGeometry component because the \"RectangleGeometry\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itRectangleGeometryComponent,
			"RectangleGeometry",
			{ "width", "height" }
		);

		// Deserialize the width and height
		if (
			!deserializeRectangleGeometryComponentWidth(*itRectangleGeometryComponent, rectangleGeometryComponent.width) ||
			!deserializeRectangleGeometryComponentHeight(*itRectangleGeometryComponent, rectangleGeometryComponent.height)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a RectangleGeometry component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the radius of a given circle geometry component JSON.
	static bool deserializeCircleGeometryComponentRadius
	(
		const json& circleGeometryComponentData,
		float& radius
	)
	{
		const auto itRadius = circleGeometryComponentData.find("radius");
		if (itRadius == circleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itRadius->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a CircleGeometry component's radius. The \"radius\" field must be a number.", "Pekan");
			return false;
		}
		radius = itRadius->get<float>();
		return true;
	}

	// Deserializes the segments count of a given circle geometry component JSON.
	static bool deserializeCircleGeometryComponentSegmentsCount
	(
		const json& circleGeometryComponentData,
		int& segmentsCount
	)
	{
		const auto itSegmentsCount = circleGeometryComponentData.find("segmentsCount");
		if (itSegmentsCount == circleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itSegmentsCount->is_number_integer())
		{
			PK_LOG_ERROR("Failed to deserialize a CircleGeometry component's segments count. The \"segmentsCount\" field must be an integer.", "Pekan");
			return false;
		}

		// The JSON library stores integers internally as either signed or unsigned values.
		// Read the wide value and validate it before converting to int,
		// as the conversion could silently truncate an out-of-range value.
		constexpr int64_t minSegmentsCount = std::numeric_limits<int>::min();
		constexpr int64_t maxSegmentsCount = std::numeric_limits<int>::max();
		if (itSegmentsCount->is_number_unsigned())
		{
			const uint64_t rawSegmentsCount = itSegmentsCount->get<uint64_t>();
			if (rawSegmentsCount > static_cast<uint64_t>(maxSegmentsCount))
			{
				PK_LOG_ERROR("Failed to deserialize a CircleGeometry component's segments count. The \"segmentsCount\" value (" << rawSegmentsCount
					<< ") must be between " << minSegmentsCount << " and " << maxSegmentsCount << ".", "Pekan");
				return false;
			}
			segmentsCount = static_cast<int>(rawSegmentsCount);
		}
		else
		{
			const int64_t rawSegmentsCount = itSegmentsCount->get<int64_t>();
			if (rawSegmentsCount < minSegmentsCount || rawSegmentsCount > maxSegmentsCount)
			{
				PK_LOG_ERROR("Failed to deserialize a CircleGeometry component's segments count. The \"segmentsCount\" value (" << rawSegmentsCount
					<< ") must be between " << minSegmentsCount << " and " << maxSegmentsCount << ".", "Pekan");
				return false;
			}
			segmentsCount = static_cast<int>(rawSegmentsCount);
		}
		return true;
	}

	// Deserializes the CircleGeometry component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeCircleGeometryComponent
	(
		const json& componentsData,
		CircleGeometryComponent& circleGeometryComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the circle geometry component
		const auto itCircleGeometryComponent = componentsData.find("CircleGeometry");
		if (itCircleGeometryComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itCircleGeometryComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a CircleGeometry component because the \"CircleGeometry\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itCircleGeometryComponent,
			"CircleGeometry",
			{ "radius", "segmentsCount" }
		);

		// Deserialize the radius and segments count
		if (
			!deserializeCircleGeometryComponentRadius(*itCircleGeometryComponent, circleGeometryComponent.radius) ||
			!deserializeCircleGeometryComponentSegmentsCount(*itCircleGeometryComponent, circleGeometryComponent.segmentsCount)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a CircleGeometry component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the point A of a given triangle geometry component JSON.
	static bool deserializeTriangleGeometryComponentPointA
	(
		const json& triangleGeometryComponentData,
		float& pointAX,
		float& pointAY
	)
	{
		const auto itPointA = triangleGeometryComponentData.find("pointA");
		if (itPointA == triangleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointA->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point A because its \"pointA\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointA->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point A. The \"pointA\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointA->at(0).is_number() || !itPointA->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point A. The \"pointA\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointAX = itPointA->at(0).get<float>();
		pointAY = itPointA->at(1).get<float>();
		return true;
	}

	// Deserializes the point B of a given triangle geometry component JSON.
	static bool deserializeTriangleGeometryComponentPointB
	(
		const json& triangleGeometryComponentData,
		float& pointBX,
		float& pointBY
	)
	{
		const auto itPointB = triangleGeometryComponentData.find("pointB");
		if (itPointB == triangleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointB->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point B because its \"pointB\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointB->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point B. The \"pointB\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointB->at(0).is_number() || !itPointB->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point B. The \"pointB\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointBX = itPointB->at(0).get<float>();
		pointBY = itPointB->at(1).get<float>();
		return true;
	}

	// Deserializes the point C of a given triangle geometry component JSON.
	static bool deserializeTriangleGeometryComponentPointC
	(
		const json& triangleGeometryComponentData,
		float& pointCX,
		float& pointCY
	)
	{
		const auto itPointC = triangleGeometryComponentData.find("pointC");
		if (itPointC == triangleGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointC->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point C because its \"pointC\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointC->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point C. The \"pointC\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointC->at(0).is_number() || !itPointC->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component's point C. The \"pointC\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointCX = itPointC->at(0).get<float>();
		pointCY = itPointC->at(1).get<float>();
		return true;
	}

	// Deserializes the TriangleGeometry component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeTriangleGeometryComponent
	(
		const json& componentsData,
		TriangleGeometryComponent& triangleGeometryComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the triangle geometry component
		const auto itTriangleGeometryComponent = componentsData.find("TriangleGeometry");
		if (itTriangleGeometryComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itTriangleGeometryComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component because the \"TriangleGeometry\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itTriangleGeometryComponent,
			"TriangleGeometry",
			{ "pointA", "pointB", "pointC" }
		);

		// Deserialize the point A, point B and point C
		if (
			!deserializeTriangleGeometryComponentPointA(*itTriangleGeometryComponent, triangleGeometryComponent.pointA.x, triangleGeometryComponent.pointA.y) ||
			!deserializeTriangleGeometryComponentPointB(*itTriangleGeometryComponent, triangleGeometryComponent.pointB.x, triangleGeometryComponent.pointB.y) ||
			!deserializeTriangleGeometryComponentPointC(*itTriangleGeometryComponent, triangleGeometryComponent.pointC.x, triangleGeometryComponent.pointC.y)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a TriangleGeometry component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the vertex positions of a given polygon geometry component JSON.
	static bool deserializePolygonGeometryComponentVertexPositions
	(
		const json& polygonGeometryComponentData,
		std::vector<glm::vec2>& vertexPositions    // output parameter where the deserialized data is filled into
	)
	{
		PK_ASSERT_QUICK(vertexPositions.empty());

		const auto itVertexPositions = polygonGeometryComponentData.find("vertexPositions");
		if (itVertexPositions == polygonGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itVertexPositions->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component's vertex positions because its \"vertexPositions\" field is not an array.", "Pekan");
			return false;
		}

		// Reserve enough memory in the vertex positions vector to hold the number of elements in the JSON array
		vertexPositions.reserve(itVertexPositions->size());
		// Deserialize each vertex position from the JSON array into the vertex positions vector
		for (size_t i = 0; i < itVertexPositions->size(); ++i)
		{
			const auto& vertexPositionData = (*itVertexPositions)[i];
			if (!vertexPositionData.is_array())
			{
				PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component's vertex positions because the \"vertexPositions\" array "
				             "contains an element which is not an array.", "Pekan");
				return false;
			}
			if (vertexPositionData.size() != 2)
			{
				PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component's vertex positions. "
					         "Each element of the \"vertexPositions\" array must contain exactly 2 elements.", "Pekan");
				return false;
			}
			if (!vertexPositionData.at(0).is_number() || !vertexPositionData.at(1).is_number())
			{
				PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component's vertex positions. "
				             "Each element of the \"vertexPositions\" array must contain numeric values only.", "Pekan");
				return false;
			}

			const glm::vec2 vertexPosition = { vertexPositionData.at(0).get<float>(), vertexPositionData.at(1).get<float>() };
			vertexPositions.push_back(vertexPosition);
		}

		return true;
	}

	// Deserializes the PolygonGeometry component from a given components JSON object, if present.
	static DeserializeComponentResult deserializePolygonGeometryComponent
	(
		const json& componentsData,
		PolygonGeometryComponent& polygonGeometryComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the polygon geometry component
		const auto itPolygonGeometryComponent = componentsData.find("PolygonGeometry");
		if (itPolygonGeometryComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itPolygonGeometryComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component because the \"PolygonGeometry\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itPolygonGeometryComponent,
			"PolygonGeometry",
			{ "vertexPositions" }
		);

		// Deserialize the vertex positions
		if (!deserializePolygonGeometryComponentVertexPositions(*itPolygonGeometryComponent, polygonGeometryComponent.vertexPositions))
		{
			PK_LOG_ERROR("Failed to deserialize a PolygonGeometry component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the point A of a given line geometry component JSON.
	static bool deserializeLineGeometryComponentPointA
	(
		const json& lineGeometryComponentData,
		float& pointAX,
		float& pointAY
	)
	{
		const auto itPointA = lineGeometryComponentData.find("pointA");
		if (itPointA == lineGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointA->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point A because its \"pointA\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointA->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point A. The \"pointA\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointA->at(0).is_number() || !itPointA->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point A. The \"pointA\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointAX = itPointA->at(0).get<float>();
		pointAY = itPointA->at(1).get<float>();
		return true;
	}

	// Deserializes the point B of a given line geometry component JSON.
	static bool deserializeLineGeometryComponentPointB
	(
		const json& lineGeometryComponentData,
		float& pointBX,
		float& pointBY
	)
	{
		const auto itPointB = lineGeometryComponentData.find("pointB");
		if (itPointB == lineGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointB->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point B because its \"pointB\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointB->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point B. The \"pointB\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointB->at(0).is_number() || !itPointB->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's point B. The \"pointB\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointBX = itPointB->at(0).get<float>();
		pointBY = itPointB->at(1).get<float>();
		return true;
	}

	// Deserializes the thickness of a given line geometry component JSON.
	static bool deserializeLineGeometryComponentThickness
	(
		const json& lineGeometryComponentData,
		float& thickness
	)
	{
		const auto itThickness = lineGeometryComponentData.find("thickness");
		if (itThickness == lineGeometryComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itThickness->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component's thickness. The \"thickness\" field must be a number.", "Pekan");
			return false;
		}
		thickness = itThickness->get<float>();
		return true;
	}

	// Deserializes the LineGeometry component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeLineGeometryComponent
	(
		const json& componentsData,
		LineGeometryComponent& lineGeometryComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the line geometry component
		const auto itLineGeometryComponent = componentsData.find("LineGeometry");
		if (itLineGeometryComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itLineGeometryComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component because the \"LineGeometry\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itLineGeometryComponent,
			"LineGeometry",
			{ "pointA", "pointB", "thickness" }
		);

		// Deserialize the point A, point B and thickness
		if (
			!deserializeLineGeometryComponentPointA(*itLineGeometryComponent, lineGeometryComponent.pointA.x, lineGeometryComponent.pointA.y) ||
			!deserializeLineGeometryComponentPointB(*itLineGeometryComponent, lineGeometryComponent.pointB.x, lineGeometryComponent.pointB.y) ||
			!deserializeLineGeometryComponentThickness(*itLineGeometryComponent, lineGeometryComponent.thickness)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a LineGeometry component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the color of a given solid color material component JSON.
	static bool deserializeSolidColorMaterialComponentColor
	(
		const json& solidColorMaterialComponentData,
		float& colorX,
		float& colorY,
		float& colorZ,
		float& colorW
	)
	{
		const auto itColor = solidColorMaterialComponentData.find("color");
		if (itColor == solidColorMaterialComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itColor->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a SolidColorMaterial component's color because its \"color\" field is not an array.", "Pekan");
			return false;
		}
		if (itColor->size() != 4)
		{
			PK_LOG_ERROR("Failed to deserialize a SolidColorMaterial component's color. The \"color\" array must contain exactly 4 elements.", "Pekan");
			return false;
		}
		if (!itColor->at(0).is_number() || !itColor->at(1).is_number() || !itColor->at(2).is_number() || !itColor->at(3).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a SolidColorMaterial component's color. The \"color\" array's elements must be numbers.", "Pekan");
			return false;
		}
		colorX = itColor->at(0).get<float>();
		colorY = itColor->at(1).get<float>();
		colorZ = itColor->at(2).get<float>();
		colorW = itColor->at(3).get<float>();
		return true;
	}

	// Deserializes the SolidColorMaterial component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeSolidColorMaterialComponent
	(
		const json& componentsData,
		SolidColorMaterialComponent& solidColorMaterialComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the solid color material component
		const auto itSolidColorMaterialComponent = componentsData.find("SolidColorMaterial");
		if (itSolidColorMaterialComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itSolidColorMaterialComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a SolidColorMaterial component because the \"SolidColorMaterial\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itSolidColorMaterialComponent,
			"SolidColorMaterial",
			{ "color" }
		);

		// Deserialize the color
		if (!deserializeSolidColorMaterialComponentColor(*itSolidColorMaterialComponent, solidColorMaterialComponent.color.x, solidColorMaterialComponent.color.y, solidColorMaterialComponent.color.z, solidColorMaterialComponent.color.w))
		{
			PK_LOG_ERROR("Failed to deserialize a SolidColorMaterial component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the point A of a given line component JSON.
	static bool deserializeLineComponentPointA
	(
		const json& lineComponentData,
		float& pointAX,
		float& pointAY
	)
	{
		const auto itPointA = lineComponentData.find("pointA");
		if (itPointA == lineComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointA->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point A because its \"pointA\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointA->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point A. The \"pointA\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointA->at(0).is_number() || !itPointA->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point A. The \"pointA\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointAX = itPointA->at(0).get<float>();
		pointAY = itPointA->at(1).get<float>();
		return true;
	}

	// Deserializes the point B of a given line component JSON.
	static bool deserializeLineComponentPointB
	(
		const json& lineComponentData,
		float& pointBX,
		float& pointBY
	)
	{
		const auto itPointB = lineComponentData.find("pointB");
		if (itPointB == lineComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPointB->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point B because its \"pointB\" field is not an array.", "Pekan");
			return false;
		}
		if (itPointB->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point B. The \"pointB\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPointB->at(0).is_number() || !itPointB->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's point B. The \"pointB\" array's elements must be numbers.", "Pekan");
			return false;
		}
		pointBX = itPointB->at(0).get<float>();
		pointBY = itPointB->at(1).get<float>();
		return true;
	}

	// Deserializes the color of a given line component JSON.
	static bool deserializeLineComponentColor
	(
		const json& lineComponentData,
		float& colorX,
		float& colorY,
		float& colorZ,
		float& colorW
	)
	{
		const auto itColor = lineComponentData.find("color");
		if (itColor == lineComponentData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itColor->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's color because its \"color\" field is not an array.", "Pekan");
			return false;
		}
		if (itColor->size() != 4)
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's color. The \"color\" array must contain exactly 4 elements.", "Pekan");
			return false;
		}
		if (!itColor->at(0).is_number() || !itColor->at(1).is_number() || !itColor->at(2).is_number() || !itColor->at(3).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component's color. The \"color\" array's elements must be numbers.", "Pekan");
			return false;
		}
		colorX = itColor->at(0).get<float>();
		colorY = itColor->at(1).get<float>();
		colorZ = itColor->at(2).get<float>();
		colorW = itColor->at(3).get<float>();
		return true;
	}

	// Deserializes the Line component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeLineComponent
	(
		const json& componentsData,
		LineComponent& lineComponent    // output parameter where the deserialized data is filled into
	)
	{
		// Get the line component
		const auto itLineComponent = componentsData.find("Line");
		if (itLineComponent == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itLineComponent->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a Line component because the \"Line\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itLineComponent,
			"Line",
			{ "pointA", "pointB", "color" }
		);

		// Deserialize the point A, point B and color
		if (
			!deserializeLineComponentPointA(*itLineComponent, lineComponent.pointA.x, lineComponent.pointA.y) ||
			!deserializeLineComponentPointB(*itLineComponent, lineComponent.pointB.x, lineComponent.pointB.y) ||
			!deserializeLineComponentColor(*itLineComponent, lineComponent.color.x, lineComponent.color.y, lineComponent.color.z, lineComponent.color.w)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a Line component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Deserializes the size of a given camera component JSON.
	static bool deserializeCameraComponent2DSize
	(
		const json& cameraComponent2DData,
		float& sizeX,
		float& sizeY
	)
	{
		const auto itSize = cameraComponent2DData.find("size");
		if (itSize == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itSize->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's size because its \"size\" field is not an array.", "Pekan");
			return false;
		}
		if (itSize->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's size. The \"size\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itSize->at(0).is_number() || !itSize->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's size. The \"size\" array's elements must be numbers.", "Pekan");
			return false;
		}
		sizeX = itSize->at(0).get<float>();
		sizeY = itSize->at(1).get<float>();
		return true;
	}

	// Deserializes the position of a given camera component JSON.
	static bool deserializeCameraComponent2DPosition
	(
		const json& cameraComponent2DData,
		float& positionX,
		float& positionY
	)
	{
		const auto itPosition = cameraComponent2DData.find("position");
		if (itPosition == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itPosition->is_array())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's position because its \"position\" field is not an array.", "Pekan");
			return false;
		}
		if (itPosition->size() != 2)
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's position. The \"position\" array must contain exactly 2 elements.", "Pekan");
			return false;
		}
		if (!itPosition->at(0).is_number() || !itPosition->at(1).is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's position. The \"position\" array's elements must be numbers.", "Pekan");
			return false;
		}
		positionX = itPosition->at(0).get<float>();
		positionY = itPosition->at(1).get<float>();
		return true;
	}

	// Deserializes the rotation of a given camera component JSON.
	static bool deserializeCameraComponent2DRotation
	(
		const json& cameraComponent2DData,
		float& rotation
	)
	{
		const auto itRotation = cameraComponent2DData.find("rotation");
		if (itRotation == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itRotation->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's rotation. The \"rotation\" field must be a number.", "Pekan");
			return false;
		}
		rotation = itRotation->get<float>();
		return true;
	}

	// Deserializes the zoom level of a given camera component JSON.
	static bool deserializeCameraComponent2DZoomLevel
	(
		const json& cameraComponent2DData,
		float& zoomLevel
	)
	{
		const auto itZoomLevel = cameraComponent2DData.find("zoomLevel");
		if (itZoomLevel == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itZoomLevel->is_number())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's zoom level. The \"zoomLevel\" field must be a number.", "Pekan");
			return false;
		}
		zoomLevel = itZoomLevel->get<float>();
		return true;
	}

	// Deserializes the isPrimary flag of a given camera component JSON.
	static bool deserializeCameraComponent2DIsPrimary
	(
		const json& cameraComponent2DData,
		bool& isPrimary
	)
	{
		const auto itIsPrimary = cameraComponent2DData.find("isPrimary");
		if (itIsPrimary == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itIsPrimary->is_boolean())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's isPrimary flag. The \"isPrimary\" field must be a boolean.", "Pekan");
			return false;
		}
		isPrimary = itIsPrimary->get<bool>();
		return true;
	}

	// Deserializes the isControllable flag of a given camera component JSON.
	static bool deserializeCameraComponent2DIsControllable
	(
		const json& cameraComponent2DData,
		bool& isControllable
	)
	{
		const auto itIsControllable = cameraComponent2DData.find("isControllable");
		if (itIsControllable == cameraComponent2DData.end())
		{
			// The field is missing, which is ok. Just use the default value.
			return true;
		}
		if (!itIsControllable->is_boolean())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component's isControllable flag. The \"isControllable\" field must be a boolean.", "Pekan");
			return false;
		}
		isControllable = itIsControllable->get<bool>();
		return true;
	}

	// Deserializes the Camera2D component from a given components JSON object, if present.
	static DeserializeComponentResult deserializeCameraComponent2D
	(
		const json& componentsData,
		CameraComponent2D& cameraComponent2D    // output parameter where the deserialized data is filled into
	)
	{
		// Get the camera 2D component
		const auto itCameraComponent2D = componentsData.find("Camera2D");
		if (itCameraComponent2D == componentsData.end())
		{
			return DeserializeComponentResult::NotFound;
		}
		if (!itCameraComponent2D->is_object())
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component because the \"Camera2D\" field is not an object.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}
		warnUnknownComponentFields
		(
			*itCameraComponent2D,
			"Camera2D",
			{ "size", "position", "rotation", "zoomLevel", "isPrimary", "isControllable" }
		);

		// Deserialize the size, position, rotation, zoom level, isPrimary flag and isControllable flag
		if (
			!deserializeCameraComponent2DSize(*itCameraComponent2D, cameraComponent2D.size.x, cameraComponent2D.size.y) ||
			!deserializeCameraComponent2DPosition(*itCameraComponent2D, cameraComponent2D.position.x, cameraComponent2D.position.y) ||
			!deserializeCameraComponent2DRotation(*itCameraComponent2D, cameraComponent2D.rotation) ||
			!deserializeCameraComponent2DZoomLevel(*itCameraComponent2D, cameraComponent2D.zoomLevel) ||
			!deserializeCameraComponent2DIsPrimary(*itCameraComponent2D, cameraComponent2D.isPrimary) ||
			!deserializeCameraComponent2DIsControllable(*itCameraComponent2D, cameraComponent2D.isControllable)
		)
		{
			PK_LOG_ERROR("Failed to deserialize a Camera2D component.", "Pekan");
			return DeserializeComponentResult::FoundAndError;
		}

		return DeserializeComponentResult::FoundAndOk;
	}

	// Checks if the given key is a known component type for a 2D scene.
	static bool isKnownScene2DComponentKey(const std::string& key)
	{
		return
			key == "Transform2D" ||
			key == "Sprite" ||
			key == "RectangleGeometry" ||
			key == "CircleGeometry" ||
			key == "TriangleGeometry" ||
			key == "PolygonGeometry" ||
			key == "LineGeometry" ||
			key == "SolidColorMaterial" ||
			key == "Line" ||
			key == "Camera2D";
	}

	// Logs a warning for each unknown component key in a given components JSON object.
	// Unknown keys are skipped, they do NOT abort the load.
	static void warnUnknownComponentKeys(const json& componentsData, const std::string& sceneType)
	{
		for (auto it = componentsData.begin(); it != componentsData.end(); ++it)
		{
			if (!isKnownScene2DComponentKey(it.key()))
			{
				PK_LOG_WARNING("Ignoring unknown component \"" << it.key()
					<< "\" because it is not valid for scene type \"" << sceneType << "\".", "Pekan");
			}
		}
	}

//////////
//////////
//////////

	json Scene2DSerializer::serializeComponents(entt::entity entity, const entt::registry& registry) const
	{
		// JSON object that will contain the serialized components data
		json componentsData = json::object();

		const TransformComponent2D* transformComponent = registry.try_get<TransformComponent2D>(entity);
		if (transformComponent != nullptr)
		{
			componentsData["Transform2D"] = serializeTransformComponent(*transformComponent, registry);
		}
		const SpriteComponent* spriteComponent = registry.try_get<SpriteComponent>(entity);
		if (spriteComponent != nullptr)
		{
			componentsData["Sprite"] = serializeSpriteComponent(*spriteComponent);
		}
		const RectangleGeometryComponent* rectangleGeometryComponent = registry.try_get<RectangleGeometryComponent>(entity);
		if (rectangleGeometryComponent != nullptr)
		{
			componentsData["RectangleGeometry"] = serializeRectangleGeometryComponent(*rectangleGeometryComponent);
		}
		const CircleGeometryComponent* circleGeometryComponent = registry.try_get<CircleGeometryComponent>(entity);
		if (circleGeometryComponent != nullptr)
		{
			componentsData["CircleGeometry"] = serializeCircleGeometryComponent(*circleGeometryComponent);
		}
		const TriangleGeometryComponent* triangleGeometryComponent = registry.try_get<TriangleGeometryComponent>(entity);
		if (triangleGeometryComponent != nullptr)
		{
			componentsData["TriangleGeometry"] = serializeTriangleGeometryComponent(*triangleGeometryComponent);
		}
		const PolygonGeometryComponent* polygonGeometryComponent = registry.try_get<PolygonGeometryComponent>(entity);
		if (polygonGeometryComponent != nullptr)
		{
			componentsData["PolygonGeometry"] = serializePolygonGeometryComponent(*polygonGeometryComponent);
		}
		const LineGeometryComponent* lineGeometryComponent = registry.try_get<LineGeometryComponent>(entity);
		if (lineGeometryComponent != nullptr)
		{
			componentsData["LineGeometry"] = serializeLineGeometryComponent(*lineGeometryComponent);
		}
		const SolidColorMaterialComponent* solidColorMaterialComponent = registry.try_get<SolidColorMaterialComponent>(entity);
		if (solidColorMaterialComponent != nullptr)
		{
			componentsData["SolidColorMaterial"] = serializeSolidColorMaterialComponent(*solidColorMaterialComponent);
		}
		const LineComponent* lineComponent = registry.try_get<LineComponent>(entity);
		if (lineComponent != nullptr)
		{
			componentsData["Line"] = serializeLineComponent(*lineComponent);
		}
		const CameraComponent2D* cameraComponent2D = registry.try_get<CameraComponent2D>(entity);
		if (cameraComponent2D != nullptr)
		{
			componentsData["Camera2D"] = serializeCameraComponent2D(*cameraComponent2D);
		}

		return componentsData;
	}

	bool Scene2DSerializer::deserializeComponents
	(
		const json& componentsJson,
		entt::entity entity,
		entt::registry& registry,
		const std::unordered_map<std::string, EntityID>& entityNameToIdMap
	) const
	{
		try
		{
			// Warn about unknown component keys and skip them.
			warnUnknownComponentKeys(componentsJson, getSceneType());

			// A variable to be reused to hold the result of deserializing each component type
			DeserializeComponentResult deserializeComponentResult;

			// Deserialize transform component.
			TransformComponent2D transformComponent;
			EntityID parentId = INVALID_ENTITY_ID;
			deserializeComponentResult = deserializeTransformComponent
			(
				componentsJson,
				entityNameToIdMap,
				transformComponent,
				parentId
			);
			// Emplace the transform component on the entity, if it was deserialized successfully,
			// together with a temporary UnresolvedTransformParentComponent2D_ carrying the unresolved parent ID,
			// which will later be resolved in postDeserialize() and the component will be removed.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<TransformComponent2D>(entity, transformComponent);
				if (parentId != INVALID_ENTITY_ID)
				{
					registry.emplace<UnresolvedTransformParentComponent2D_>(entity, parentId);
				}
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the Transform2D component is invalid.", "Pekan");
				return false;
			}

			// Deserialize sprite component.
			SpriteComponent spriteComponent;
			deserializeComponentResult = deserializeSpriteComponent(componentsJson, spriteComponent);
			// Emplace the sprite component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<SpriteComponent>(entity, spriteComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the Sprite component is invalid.", "Pekan");
				return false;
			}

			// Deserialize rectangle geometry component.
			RectangleGeometryComponent rectangleGeometryComponent;
			deserializeComponentResult = deserializeRectangleGeometryComponent(componentsJson, rectangleGeometryComponent);
			// Emplace the rectangle geometry component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<RectangleGeometryComponent>(entity, rectangleGeometryComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the RectangleGeometry component is invalid.", "Pekan");
				return false;
			}

			// Deserialize circle geometry component.
			CircleGeometryComponent circleGeometryComponent;
			deserializeComponentResult = deserializeCircleGeometryComponent(componentsJson, circleGeometryComponent);
			// Emplace the circle geometry component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<CircleGeometryComponent>(entity, circleGeometryComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the CircleGeometry component is invalid.", "Pekan");
				return false;
			}

			// Deserialize triangle geometry component.
			TriangleGeometryComponent triangleGeometryComponent;
			deserializeComponentResult = deserializeTriangleGeometryComponent(componentsJson, triangleGeometryComponent);
			// Emplace the triangle geometry component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<TriangleGeometryComponent>(entity, triangleGeometryComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the TriangleGeometry component is invalid.", "Pekan");
				return false;
			}

			// Deserialize polygon geometry component.
			PolygonGeometryComponent polygonGeometryComponent;
			deserializeComponentResult = deserializePolygonGeometryComponent(componentsJson, polygonGeometryComponent);
			// Emplace the polygon geometry component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<PolygonGeometryComponent>(entity, polygonGeometryComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the PolygonGeometry component is invalid.", "Pekan");
				return false;
			}

			// Deserialize line geometry component.
			LineGeometryComponent lineGeometryComponent;
			deserializeComponentResult = deserializeLineGeometryComponent(componentsJson, lineGeometryComponent);
			// Emplace the line geometry component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<LineGeometryComponent>(entity, lineGeometryComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the LineGeometry component is invalid.", "Pekan");
				return false;
			}

			// Deserialize solid color material component.
			SolidColorMaterialComponent solidColorMaterialComponent;
			deserializeComponentResult = deserializeSolidColorMaterialComponent(componentsJson, solidColorMaterialComponent);
			// Emplace the solid color material component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<SolidColorMaterialComponent>(entity, solidColorMaterialComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the SolidColorMaterial component is invalid.", "Pekan");
				return false;
			}

			// Deserialize line component.
			LineComponent lineComponent;
			deserializeComponentResult = deserializeLineComponent(componentsJson, lineComponent);
			// Emplace the line component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<LineComponent>(entity, lineComponent);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the Line component is invalid.", "Pekan");
				return false;
			}

			// Deserialize camera 2D component.
			CameraComponent2D cameraComponent2D;
			deserializeComponentResult = deserializeCameraComponent2D(componentsJson, cameraComponent2D);
			// Emplace the camera 2D component on the entity, if it was deserialized successfully.
			if (deserializeComponentResult == DeserializeComponentResult::FoundAndOk)
			{
				registry.emplace<CameraComponent2D>(entity, cameraComponent2D);
			}
			else if (deserializeComponentResult == DeserializeComponentResult::FoundAndError)
			{
				PK_LOG_ERROR("Failed to deserialize components of an entity, because the Camera2D component is invalid.", "Pekan");
				return false;
			}

			return true;
		}
		catch (const json::parse_error& e)
		{
			PK_LOG_ERROR("Failed to deserialize a components JSON containing invalid JSON syntax (JSON parse error: "
				<< e.what() << ").", "Pekan");
		}
		catch (const json::exception& e)
		{
			PK_LOG_ERROR("Failed to deserialize a scene. Scene file contains invalid data (JSON error: "
				<< e.what() << ").", "Pekan");
		}

		return false;
	}

	void Scene2DSerializer::postDeserialize(Scene& scene) const
	{
		// TODO: implement
	}

} // namespace Renderer2D
} // namespace Pekan
