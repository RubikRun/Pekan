#pragma once

#include "Scene.h"
#include "CameraController2D.h"

namespace Pekan
{
namespace Renderer2D {

	class Scene2D : public Scene
	{
		bool init() override final;
		void exit() override final;
		// Can be implemented by derived classes with additional initialization logic
		virtual bool _init() { return true; }
		// Can be implemented by derived classes with additional exit logic
		virtual void _exit() {}

		void render() const override final;
		// Can be implemented by derived classes with additional render logic
		virtual void _render() const {}

	public: /* functions */

		Scene2D(PekanApplication* application) : Scene(application) {}

		// Returns the camera controller used to allow user to control scene's camera with the mouse
		CameraController2D_Ptr getCameraController() { return m_cameraController; }

	private: /* variables */

		// A camera controller for controlling the scene's camera with the mouse
		CameraController2D_Ptr m_cameraController;
	};

} // namespace Renderer2D
} // namespace Pekan