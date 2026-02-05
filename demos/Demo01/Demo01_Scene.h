#pragma once

#include "Layer.h"
#include "DrawObject.h"

#include "Demo01_GUIWindow.h"

namespace Demo
{

	class Demo01_Scene : public Pekan::Layer
	{
	public:

		Demo01_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		// Attaches a GUI window for controlling squares
		void attachGUIWindow(const std::shared_ptr<const Demo01_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		// Generates vertices for the square with given index
		// and fills them to the vertices list.
		void generateSquareVertices(size_t idx);

		// Moves third square slightly.
		// More precisely updates third square's position based on current time.
		void moveThirdSquare();

	private: /* variables */

		Pekan::Graphics::DrawObject m_drawObject;

		std::shared_ptr<const Demo01_GUIWindow> m_guiWindow;

		// Array of vertex data of current squares
		std::vector<float> m_vertices;

		float t = 0.0f;
	};

} // namespace Demo