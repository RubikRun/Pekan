#pragma once

#include "Layer.h"

#include "Demo07_GUIWindow.h"

#include "Camera2D.h"
#include "LineShape.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "CircleShapeStatic.h"
#include "TriangleShape.h"
#include "PolygonShape.h"
#include <vector>

namespace Demo
{

	class Demo07_Scene : public Pekan::Layer
	{

	public:

		Demo07_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		void attachGUIWindow(const std::shared_ptr<const Demo07_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		void createCamera();
		void createCoordSys();

	private: /* variables */

		std::shared_ptr<const Demo07_GUIWindow> m_guiWindow;

		Pekan::Renderer::Camera2D_Ptr m_camera;
		std::vector<Pekan::Renderer::LineShape> m_coordSys;

		Pekan::Renderer::RectangleShape m_rectangle;
		Pekan::Renderer::CircleShape m_circle;
		Pekan::Renderer::CircleShapeStatic<> m_circleStatic;
		Pekan::Renderer::TriangleShape m_triangle;
		Pekan::Renderer::PolygonShape m_polygon;
		Pekan::Renderer::LineShape m_line;
	};

} // namespace Demo