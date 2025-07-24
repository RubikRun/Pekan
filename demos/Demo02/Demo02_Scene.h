#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "Demo02_GUIWindow.h"

namespace Demo
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	class Demo02_Scene : public Pekan::Layer
	{
	public:

		Demo02_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		// Attaches a GUI window for controlling scene's parameters
		void attachGUIWindow(const std::shared_ptr<const Demo02_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		// Cube's vertices
		std::vector<Vertex> m_vertices;

		bool m_hideFourthFaceCache = false;

		// Transform matrices
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;

		glm::ivec2 m_resolution;

		std::shared_ptr<const Demo02_GUIWindow> m_guiWindow;
	};

} // namespace Demo