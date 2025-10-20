#pragma once

#include "Layer.h"
#include "Line.h"
#include "Shapes/LineShape.h"
#include "Camera2D.h"

#include "Demo05_GUIWindow.h"

#define MTT0_SUBDIVS 200
#define MTT1_SUBDIVS 400

namespace Demo
{

	class Demo05_Scene : public Pekan::Layer
	{
	public:

		Demo05_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const Demo05_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void updateMtt();

	private: /* variables */

		Pekan::Renderer2D::Line m_lines[MTT0_SUBDIVS];

		Pekan::Renderer2D::LineShape m_lineShapes[MTT1_SUBDIVS];

		// GUI window for controlling background's color
		std::shared_ptr<const Demo05_GUIWindow> m_guiWindow;

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		// Multiplication factor of the 2 Mandelbrot Times Tables
		float m_mtt0factor = 30.0f;
		float m_mtt1factor = 90.0f;

		float t = 0.0f;
	};

} // namespace Demo