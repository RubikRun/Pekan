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

	void EditorScene::addComponent(entt::entity entity, int componentTypeIndex)
	{
		switch (componentTypeIndex)
		{
			case 0: m_registry.emplace<TransformComponent2D>(entity); break;
			case 1: m_registry.emplace<SpriteComponent>(entity); break;
			case 2: m_registry.emplace<CameraComponent2D>(entity); break;
			case 3: m_registry.emplace<LineComponent>(entity); break;
			case 4: m_registry.emplace<RectangleGeometryComponent>(entity); break;
			case 5: m_registry.emplace<CircleGeometryComponent>(entity); break;
			case 6: m_registry.emplace<TriangleGeometryComponent>(entity); break;
			case 7: m_registry.emplace<PolygonGeometryComponent>(entity); break;
			case 8: m_registry.emplace<LineGeometryComponent>(entity); break;
			case 9: m_registry.emplace<SolidColorMaterialComponent>(entity); break;

			default:
				PK_LOG_ERROR("Invalid component type index: " + std::to_string(componentTypeIndex), "Pekan");
				break;
		}
	}

} // namespace Editor
} // namespace Pekan
