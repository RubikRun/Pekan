#pragma once

#include "Scene2D.h"

#include <vector>

namespace Demo
{

	class Demo06_GUIWindow;

	struct BoundingBox2D
	{
		glm::vec2 min = glm::vec2(0.0f, 0.0f);
		glm::vec2 max = glm::vec2(0.0f, 0.0f);
		glm::vec2 size = glm::vec2(0.0f, 0.0f);
		glm::vec2 center = glm::vec2(0.0f, 0.0f);
	};

	class Demo06_Scene : public Pekan::Renderer2D::Scene2D
	{

	public:

		Demo06_Scene(Pekan::PekanApplication* application) : Scene2D(application) {}
		
		void attachGUIWindow(const std::shared_ptr<Demo06_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

		void createBbox();
		void createCameras();

		void createShapes();
		void createRectangles();
		void createCircles();
		void createTriangles();
		void createPolygons();
		void createLines();
		void createCenterSquare();

		void updateShapes(float dt, bool perShapeTypeCountChanged);

		// Updates enabled/disabled state of each entity in a given vector of shapes of a single shape type,
		// based on current perShapeTypeCount and whether this shape type is enabled in GUI.
		//
		// @param[in] shapes - Vector of entities representing shapes of a single shape type
		// @param[in] enabled - Is this shape type enabled in GUI
		void updateDisabledComponentOfShapeType(std::vector<entt::entity>& shapes, bool enabled);

		void updateRectangles(float dt);
		void updateCircles(float dt);
		void updateTriangles(float dt);
		void updatePolygons(float dt);
		void updateLines(float dt);

		// Updates post-processing shader
		void updatePps();

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		std::shared_ptr<Demo06_GUIWindow> m_guiWindow;

		std::vector<entt::entity> m_rectangles;
		std::vector<entt::entity> m_circles;
		std::vector<entt::entity> m_triangles;
		std::vector<entt::entity> m_polygons;
		std::vector<entt::entity> m_lines;

		bool m_prevRectanglesEnabled = false;
		bool m_prevCirclesEnabled = false;
		bool m_prevTrianglesEnabled = false;
		bool m_prevPolygonsEnabled = false;
		bool m_prevLinesEnabled = false;

		entt::entity m_centerSquare = entt::null;

		entt::entity m_cameraFirst = entt::null;
		entt::entity m_cameraSecond = entt::null;

		int m_currentCameraIdx = 0;

		BoundingBox2D m_bbox;

		int m_perShapeTypeCount = -1;
		int m_prevPerShapeTypeCount = -1;
		int m_perShapeTypeMaxCount = -1;

		float t = 0.0f;

		// Cached ECS registry reference
		entt::registry& m_registry = getRegistry();
	};

} // namespace Demo