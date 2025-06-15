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
	};

	class Demo06_Scene : public Pekan::Layer
	{
		using CameraPtr = std::shared_ptr<Pekan::Renderer::Camera2D>;

	public:

		Demo06_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		void attachGUIWindow(const Demo06_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		void createBbox();
		void createCameras();
		void createShapes();
		void createRectangles();

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		std::vector<Pekan::Renderer::RectangleShape> m_rectangles;
		std::vector<Pekan::Renderer::CircleShape> m_circles;
		std::vector<Pekan::Renderer::CircleShapeStatic<>> m_circlesStatic;
		std::vector<Pekan::Renderer::TriangleShape> m_triangles;
		std::vector<Pekan::Renderer::PolygonShape> m_polygons;
		std::vector<Pekan::Renderer::LineShape> m_lines;

		// A square in the center of the bounding box, just to make sure that coordinates are resolution-independent
		Pekan::Renderer::RectangleShape m_centerSquare;

		CameraPtr m_cameraFirst;
		CameraPtr m_cameraSecond;

		int m_currentCameraIdx = 0;

		BoundingBox2D m_bbox;

		int m_perShapeTypeCount = -1;

		glm::vec2 m_mousePos = glm::vec2(0.0f, 0.0f);

		const Demo06_GUIWindow* m_guiWindow = nullptr;
	};

} // namespace Demo