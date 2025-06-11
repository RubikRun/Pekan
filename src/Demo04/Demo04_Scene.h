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

		Demo04_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo04_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		void updatePolygon();

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		Pekan::Renderer::TriangleShape m_triangle;
		Pekan::Renderer::RectangleShape m_rectangle;
		Pekan::Renderer::CircleShape m_circle;
		Pekan::Renderer::CircleShapeStatic<> m_circleStatic;
		Pekan::Renderer::PolygonShape m_polygon;

		glm::vec2 m_rectangleInitialPosition;
		float m_rectangleInitialWidth;
		float m_rectangleInitialHeight;

		glm::vec2 m_circleInitialPosition;
		glm::vec2 m_circleStaticInitialPosition;
		float m_circleInitialRadius;
		float m_circleStaticInitialRadius;

		glm::vec2 m_polygonInitialPosition;

		int m_shaderIdx = -1;

		Pekan::Renderer::Image m_image0;
		Pekan::Renderer::Image m_image1;

		const Demo04_GUIWindow* m_guiWindow = nullptr;

		bool m_enabledFaceCulling = false;

		float t = 0.0f;
	};

} // namespace Demo