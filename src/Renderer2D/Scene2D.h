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

	public: /* functions */

		Scene2D(PekanApplication* application) : Scene(application) {}
		void render() const override final;

		// Returns the camera controller used to allow user to control scene's camera with the mouse
		CameraController2D_Ptr getCameraController() { return m_cameraController; }

	protected: /* functions */

		// Can be implemented by derived classes with specific render logic
		virtual void _render() const {}
		// Can be implemented by derived classes with specific initialization logic
		virtual bool _init() { return true; }
		// Can be implemented by derived classes with specific exit logic
		virtual void _exit() {}

	private: /* variables */

		// A camera controller for controlling the scene's camera with the mouse
		CameraController2D_Ptr m_cameraController;
	};

} // namespace Renderer2D
} // namespace Pekan