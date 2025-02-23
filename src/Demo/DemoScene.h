#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "DemoGUIWindow.h"

namespace Demo
{

	class DemoScene : public Pekan::PekanScene
	{
	public:

		void update() override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const DemoGUIWindow* guiWindow) { this->guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;

	private: /* variables */

		Pekan::Renderer::VertexBuffer vertexBuffer;

		Pekan::Renderer::VertexArray vertexArray;

		Pekan::Renderer::Shader shader;

		// Element buffer object ID
		unsigned ebo;

		// GUI window for controlling background's color
		const DemoGUIWindow* guiWindow = nullptr;
	};

} // namespace Demo