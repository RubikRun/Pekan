#pragma once

#include "Layer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Demo00_GUIWindow.h"

namespace Demo
{

	class Demo00_Scene : public Pekan::Layer
	{
	public:

		Demo00_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo00_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::Shader m_shader;

		// GUI window for controlling background's color
		const Demo00_GUIWindow* m_guiWindow = nullptr;
	};

} // namespace Demo