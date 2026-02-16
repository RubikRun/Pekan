#include "EditorScene.h"

#include "RenderState.h"
#include "PekanLogger.h"

#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "CameraComponent2D.h"
#include "LineComponent.h"
#include "SolidColorMaterialComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "LineGeometryComponent.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;
using namespace Pekan;

namespace Pekan
{
namespace Editor
{

	// Names of component types supported in the editor
	static const std::vector<std::string> s_componentTypesNames =
	{
		"Transform2D",
		"Sprite",
		"Camera2D",
		"Line",
		"RectangleGeometry",
		"CircleGeometry",
		"TriangleGeometry",
		"PolygonGeometry",
		"LineGeometry",
		"SolidColorMaterial"
	};

	bool EditorScene::_init()
	{
		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		return true;
	}

	void EditorScene::_exit()
	{
	}

	void EditorScene::update(double deltaTime)
	{
	}

	void EditorScene::addEntity()
	{
		const entt::entity entity = createEntity();
		m_entities.push_back(entity);
	}

	const std::vector<std::string>& EditorScene::getComponentTypesNames()
	{
		return s_componentTypesNames;
	}

	// Adds a component of the given type to a given entity if it doesn't already have that component
	template<typename ComponentType>
	void addComponentOfType(entt::registry& registry, entt::entity entity, int componentTypeIndex)
	{
		if (registry.all_of<ComponentType>(entity))
		{
			PK_LOG_WARNING("Entity " + std::to_string(uint32_t(entity)) + " already has component of type " + s_componentTypesNames[componentTypeIndex] + ".", "Pekan");
			return;
		}

		registry.emplace<ComponentType>(entity);
	}

	void EditorScene::addComponent(entt::entity entity, int componentTypeIndex)
	{
		switch (componentTypeIndex)
		{
			case 0: addComponentOfType<TransformComponent2D>(m_registry, entity, componentTypeIndex); break;
			case 1: addComponentOfType<SpriteComponent>(m_registry, entity, componentTypeIndex); break;
			case 2: addComponentOfType<CameraComponent2D>(m_registry, entity, componentTypeIndex); break;
			case 3: addComponentOfType<LineComponent>(m_registry, entity, componentTypeIndex); break;
			case 4: addComponentOfType<RectangleGeometryComponent>(m_registry, entity, componentTypeIndex); break;
			case 5: addComponentOfType<CircleGeometryComponent>(m_registry, entity, componentTypeIndex); break;
			case 6: addComponentOfType<TriangleGeometryComponent>(m_registry, entity, componentTypeIndex); break;
			case 7: addComponentOfType<PolygonGeometryComponent>(m_registry, entity, componentTypeIndex); break;
			case 8: addComponentOfType<LineGeometryComponent>(m_registry, entity, componentTypeIndex); break;
			case 9: addComponentOfType<SolidColorMaterialComponent>(m_registry, entity, componentTypeIndex); break;
			default:
				PK_LOG_ERROR("Invalid component type index: " + std::to_string(componentTypeIndex), "Pekan");
				break;
		}
	}

} // namespace Editor
} // namespace Pekan
