#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "Demo01_GUIWindow.h"

namespace Demo
{

	class Demo01_Scene : public Pekan::Layer
	{
	public:

		Demo01_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;

		// Attaches a GUI window for controlling squares
		void attachGUIWindow(const std::shared_ptr<const Demo01_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		// Generates vertices for the square with given index
		// and fills them to the vertices list.
		void generateSquareVertices(size_t idx);

		// Moves third square slightly.
		// More precisely updates third square's position based on current time.
		void moveThirdSquare();

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		std::shared_ptr<const Demo01_GUIWindow> m_guiWindow;

		// Array of vertex data of current squares
		std::vector<float> m_vertices;

		float t = 0.0f;
	};

} // namespace Demo