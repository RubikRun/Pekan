#include "PekanEngine.h"
#include "Events/MouseEvents.h"
#include "PekanTools.h"

#include "PekanApplication.h"
#include "Events/EventListener.h"

static const float ZOOM_SPEED = 1.1f;

namespace Pekan
{
namespace Tools
{


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////// class CameraController2D //////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    class CameraController2D : public EventListener
    {
        using CameraSharedPtr = std::shared_ptr<Renderer::Camera2D>;
        using CameraWeakPtr = std::weak_ptr<Renderer::Camera2D>;

    public:

        CameraController2D() = default;

        bool onMouseMoved(const MouseMovedEvent& event) override
        {
            if (!m_enabled)
            {
                return false;
            }

            CameraSharedPtr camera = m_camera.lock();
            // Check if camera still exists
            if (camera == nullptr)
            {
                return false;
            }

            // Calculate how much the mouse has moved from its previous position to the current position
            const glm::vec2 newMousePos = { event.getX(), event.getY() };
            const glm::vec2 mouseDelta = newMousePos - m_mousePos;
            // Update cached mouse position
            m_mousePos = newMousePos;

            if (PekanEngine::isMouseButtonPressed(MouseButton::Left))
            {
                // Move camera by the amount that the mouse has moved, divided by the zoom level
                camera->move(glm::vec2(-mouseDelta.x, mouseDelta.y) / camera->getZoom());
            }

            return true;
        }

        bool onMouseScrolled(const MouseScrolledEvent& event) override
        {
            if (!m_enabled)
            {
                return false;
            }

            CameraSharedPtr camera = m_camera.lock();
            // Check if camera still exists
            if (camera == nullptr)
            {
                return false;
            }

            // NOTE: Scroll amount will be mostly 1.0 or -1.0.
            //       If scrolled really fast it could sometimes be 2.0 or -2.0.
            //       We could multiply the zoom speed by this scroll amount to achieve faster zoom when scrolling faster,
            //       but it doesn't feel natural. It feels better to have a constant zoom speed no matter how fast you scroll.
            const float scrollAmount = event.getYOffset();
            if (scrollAmount > 0.0f)
            {
                camera->zoomIn(ZOOM_SPEED);
            }
            else if (scrollAmount < 0.0f)
            {
                camera->zoomOut(ZOOM_SPEED);
            }

            return true;
        }

        // Enables/disables the camera controller
        inline void setEnabled(bool enabled) { m_enabled = enabled; }

        // Sets a new camera to be controlled by the camera controller
        inline void setCamera(const CameraSharedPtr& camera) { m_camera = camera; }

    private:

        // A pointer to the camera being controlled by the camera controller.
        // NOTE: It's a weak pointer so the camera is NOT owned by the camera controller.
        //       If the camera is destroyed at some point, the camera controller will safely stop operating on it.
        CameraWeakPtr m_camera;

        glm::vec2 m_mousePos = glm::vec2(-1.0f, -1.0f);

        bool m_enabled = true;
    };

    // A global static 2D camera controller.
    // 
    // WARNING: The logic here rests on the fact that we support only 1 application at a time.
    //          If sometime in the future we support more than 1 application at a time,
    //          we'd need to rework this to be a map from application ID to 2D camera controller.
    static std::shared_ptr<CameraController2D> s_cameraController2D = std::make_shared<CameraController2D>();

    // A flag indicating if we have already registered the 2D camera controller as an event listener inside the application
    static bool s_didRegisterCameraController2D = false;


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////// PekanTools functions //////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


	void Tools::PekanTools::enableCameraController2D(const Camera2DSharedPtr& camera)
	{
        s_cameraController2D->setCamera(camera);
        s_cameraController2D->setEnabled(true);

        if (!s_didRegisterCameraController2D)
        {
            PekanApplication* application = PekanEngine::getApplication();
            if (!application)
            {
                PK_LOG_ERROR("Trying to enable 2D camera controller but there is no application registered in PekanEngine", "Pekan");
                return;
            }
            application->registerEventListener(s_cameraController2D);
            s_didRegisterCameraController2D = true;
        }
	}

	void PekanTools::disableCameraController2D()
	{
        s_cameraController2D->setEnabled(false);
	}

} // namespace Tools
} // namespace Pekan