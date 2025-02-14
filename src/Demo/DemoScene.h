#pragma once

#include "PekanScene.h"
using Pekan::PekanScene;
#include "DemoGUIWindow.h"

namespace Demo
{

	class DemoScene : public PekanScene
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

		// Shader program ID
		unsigned shaderProgram;
		// Vertex array object ID
		unsigned vao;
		// Vertex buffer object ID
		unsigned vbo;
		// Element buffer object ID
		unsigned ebo;

		// GUI window for controlling background's color
		const DemoGUIWindow* guiWindow = nullptr;
	};

} // namespace Demo