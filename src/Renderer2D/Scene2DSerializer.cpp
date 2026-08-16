#include "Scene2DSerializer.h"

#include "Scene.h"
#include "Entity/EntityIDComponent.h"
#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"

#include "Utils/SerializationUtils.h" // IWYU pragma: keep

using json = nlohmann::ordered_json;

namespace Pekan
{
namespace Renderer2D {

	// Serializes a given transform component into a JSON object
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

	// Serializes a given sprite component into a JSON object
	static json serializeSpriteComponent(const SpriteComponent& spriteComponent)
	{
		const json spriteData =
		{
			{ "width", spriteComponent.width },
			{ "height", spriteComponent.height },
			// NOTE: There is no way to evaluate the correct texture path yet because we don't store the source path in Texture2D.
			//	     For now we'll always set texturePath to null explicitly, instead of just skipping it,
			//	     to remember later that it needs to be implemented.
			{ "texturePath", nullptr },
			{ "textureCoordinatesMin", spriteComponent.textureCoordinatesMin },
			{ "textureCoordinatesMax", spriteComponent.textureCoordinatesMax }
		};
		return spriteData;
	}

	// Serializes a given rectangle geometry component into a JSON object
	static json serializeRectangleGeometryComponent(const RectangleGeometryComponent& rectangleGeometryComponent)
	{
		const json rectangleData =
		{
			{ "width", rectangleGeometryComponent.width },
			{ "height", rectangleGeometryComponent.height }
		};
		return rectangleData;
	}

	// Serializes a given circle geometry component into a JSON object
	static json serializeCircleGeometryComponent(const CircleGeometryComponent& circleGeometryComponent)
	{
		const json circleData =
		{
			{ "radius", circleGeometryComponent.radius },
			{ "segmentsCount", circleGeometryComponent.segmentsCount }
		};
		return circleData;
	}

	// Serializes a given triangle geometry component into a JSON object
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

	// Serializes a given polygon geometry component into a JSON object
	static json serializePolygonGeometryComponent(const PolygonGeometryComponent& polygonGeometryComponent)
	{
		const json polygonData =
		{
			{ "vertexPositions", polygonGeometryComponent.vertexPositions }
		};
		return polygonData;
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

		return componentsData;
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
