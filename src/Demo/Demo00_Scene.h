#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Demo00_GUIWindow.h"

namespace Demo
{

	class Demo00_Scene : public Pekan::PekanScene
	{
	public:

		void update() override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo00_GUIWindow* guiWindow) { this->guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;

	private: /* variables */

		Pekan::Renderer::VertexBuffer vertexBuffer;

		Pekan::Renderer::IndexBuffer indexBuffer;

		Pekan::Renderer::VertexArray vertexArray;

		Pekan::Renderer::Shader shader;

		// GUI window for controlling background's color
		const Demo00_GUIWindow* guiWindow = nullptr;
	};

} // namespace Demo