#pragma once

#include "Layer.h"
#include "RenderObject.h"
#include "TriangleShape.h"

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

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		Pekan::Renderer::TriangleShape m_triangle;

		glm::vec2 m_triangleInitialPosition;

		// GUI window for controlling background's color
		const Demo04_GUIWindow* m_guiWindow = nullptr;

		float t = 0.0f;
	};

} // namespace Demo