#pragma once

#include "Scene2D.h"
#include "DrawObject.h"
#include "Image.h"

#include "Demo04_GUIWindow.h"

namespace Demo
{

	class Demo04_Scene : public Pekan::Renderer2D::Scene2D
	{
	public:

		Demo04_Scene(Pekan::PekanApplication* application) : Scene2D(application) {}
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const Demo04_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;

		void _exit() override;

		void update(double deltaTime) override;

		// Override Scene2D's _render() function to manually render m_drawObject
		// since it's not part of ECS and will not be rendered automatically.
		//
		// TODO: think about if this is a good idea long-term, or if we should think of something else for such cases.
		void _render() const override;

		void updatePolygon1();
		void updatePolygon2();

	private: /* variables */

		std::shared_ptr<const Demo04_GUIWindow> m_guiWindow;

		Pekan::Graphics::DrawObject m_drawObject;

		entt::entity m_triangle = entt::null;
		entt::entity m_rectangle = entt::null;
		entt::entity m_circle = entt::null;
		entt::entity m_polygon1 = entt::null;
		entt::entity m_polygon2 = entt::null;

		glm::vec2 m_rectangleInitialPosition;
		float m_rectangleInitialWidth;
		float m_rectangleInitialHeight;

		glm::vec2 m_circleInitialPosition;
		float m_circleInitialRadius;

		glm::vec2 m_polygon1InitialPosition;
		glm::vec2 m_polygon2InitialPosition;

		int m_shaderIdx = -1;

		Pekan::Graphics::Image m_image0;
		Pekan::Graphics::Image m_image1;

		bool m_enabledFaceCulling = false;

		// The state of "enable shapes" from GUI in the previous frame
		bool m_prevIsEnabledShapes = false;

		float t = 0.0f;

		// Cached ECS registry reference
		entt::registry& m_registry = getRegistry();
	};

} // namespace Demo