#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "Demo01_GUIWindow.h"

namespace Demo
{

	struct Rectangle {
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int rotation = 0;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		int id = -1;
	};

	class Demo01_Scene : public Pekan::Layer
	{
	public:

		Demo01_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;

		void addSquare();

		// Returns a (const) reference to the list of squares
		inline const std::vector<Rectangle>& getSquares() const { return m_squares; }
		inline std::vector<Rectangle>& getSquares() { return m_squares; }

		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo01_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		const Demo01_GUIWindow* m_guiWindow = nullptr;

		// List holding currently existing squares
		std::vector<Rectangle> m_squares;
		// Array of vertex data of currently existing squares
		std::vector<float> m_vertices;

		float t = 0.0f;

		glm::ivec2 m_resolution;
	};

} // namespace Demo