#pragma once

#include "Layer.h"
#include "RenderObject.h"
#include "TriangleShape.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "CircleShapeStatic.h"
#include "PolygonShape.h"
#include "Image.h"

#include "Demo04_GUIWindow.h"

namespace Demo
{

	class Demo04_Scene : public Pekan::Layer
	{
	public:

		Demo04_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const Demo04_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void updatePolygon1();
		void updatePolygon2();

	private: /* variables */

		Pekan::Graphics::RenderObject m_renderObject;

		Pekan::Renderer2D::TriangleShape m_triangle;
		Pekan::Renderer2D::RectangleShape m_rectangle;
		Pekan::Renderer2D::CircleShape m_circle;
		Pekan::Renderer2D::CircleShapeStatic<> m_circleStatic;
		Pekan::Renderer2D::PolygonShape m_polygon1;
		Pekan::Renderer2D::PolygonShape m_polygon2;

		glm::vec2 m_rectangleInitialPosition;
		float m_rectangleInitialWidth;
		float m_rectangleInitialHeight;

		glm::vec2 m_circleInitialPosition;
		glm::vec2 m_circleStaticInitialPosition;
		float m_circleInitialRadius;
		float m_circleStaticInitialRadius;

		glm::vec2 m_polygon1InitialPosition;
		glm::vec2 m_polygon2InitialPosition;

		int m_shaderIdx = -1;

		Pekan::Graphics::Image m_image0;
		Pekan::Graphics::Image m_image1;

		std::shared_ptr<const Demo04_GUIWindow> m_guiWindow;

		bool m_enabledFaceCulling = false;

		float t = 0.0f;
	};

} // namespace Demo