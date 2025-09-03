#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "RectangleShape.h"
#include "CircleShape.h"
#include "CircleShapeStatic.h"
#include "TriangleShape.h"
#include "PolygonShape.h"
#include "LineShape.h"

#include "Camera2D.h"

#include "Demo06_GUIWindow.h"

#include <vector>

namespace Demo
{

	struct BoundingBox2D
	{
		glm::vec2 min = glm::vec2(0.0f, 0.0f);
		glm::vec2 max = glm::vec2(0.0f, 0.0f);
		glm::vec2 size = glm::vec2(0.0f, 0.0f);
		glm::vec2 center = glm::vec2(0.0f, 0.0f);
	};

	class Demo06_Scene : public Pekan::Layer
	{

	public:

		Demo06_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		void attachGUIWindow(const std::shared_ptr<Demo06_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void createBbox();
		void createCameras();

		void createShapes();
		void createRectangles();
		void createCircles();
		void createCirclesStatic();
		void createTriangles();
		void createPolygons();
		void createLines();
		void createCenterSquare();

		void updateShapes(float dt);
		void updateRectangles(float dt);
		void updateCircles(float dt);
		void updateCirclesStatic(float dt);
		void updateTriangles(float dt);
		void updatePolygons(float dt);
		void updateLines(float dt);

		// Updates post-processing shader
		void updatePps();

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		std::vector<Pekan::Renderer2D::RectangleShape> m_rectangles;
		std::vector<Pekan::Renderer2D::CircleShape> m_circles;
		std::vector<Pekan::Renderer2D::CircleShapeStatic<>> m_circlesStatic;
		std::vector<Pekan::Renderer2D::TriangleShape> m_triangles;
		std::vector<Pekan::Renderer2D::PolygonShape> m_polygons;
		std::vector<Pekan::Renderer2D::LineShape> m_lines;

		// A square in the center of the bounding box, just to make sure that coordinates are resolution-independent
		Pekan::Renderer2D::RectangleShape m_centerSquare;

		Pekan::Renderer2D::Camera2D_Ptr m_cameraFirst;
		Pekan::Renderer2D::Camera2D_Ptr m_cameraSecond;

		int m_currentCameraIdx = 0;

		BoundingBox2D m_bbox;

		int m_perShapeTypeCount = -1;
		int m_perShapeTypeMaxCount = -1;

		std::shared_ptr<Demo06_GUIWindow> m_guiWindow;

		float t = 0.0f;
	};

} // namespace Demo