#pragma once

#include "Layer.h"

#include "Demo07_GUIWindow.h"

#include "Camera2D.h"
#include "LineShape.h"
#include "RectangleShape.h"
#include <vector>

namespace Demo
{

	class Demo07_Scene : public Pekan::Layer
	{
		using CameraPtr = std::shared_ptr<Pekan::Renderer::Camera2D>;

	public:

		Demo07_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		void attachGUIWindow(const Demo07_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		void createCamera();
		void createCoordSys();

	private: /* variables */

		const Demo07_GUIWindow* m_guiWindow = nullptr;

		CameraPtr m_camera;
		std::vector<Pekan::Renderer::LineShape> m_coordSys;

		Pekan::Renderer::RectangleShape m_rectangle;
	};

} // namespace Demo