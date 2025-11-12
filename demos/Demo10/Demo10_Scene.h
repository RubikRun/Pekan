#pragma once

#include "Scene2D.h"
#include "Player.h"
#include "Camera2D.h"

namespace Demo
{

    class Demo10_GUIWindow;

    class Demo10_Scene : public Pekan::Renderer2D::Scene2D
    {
    public:

        Demo10_Scene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

        void attachGUIWindow(const std::shared_ptr<Demo10_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

        const std::shared_ptr<Demo10_GUIWindow>& getGUIWindow() const { return m_guiWindow; }

    private: /* functions */

        bool _init() override;
        void _exit() override;

        void update(double deltaTime) override;

        void createCamera();

        bool onMouseButtonPressed(const Pekan::MouseButtonPressedEvent& event) override;

    private: /* variables */

        Player m_player;

        entt::entity m_camera = entt::null;

        std::shared_ptr<Demo10_GUIWindow> m_guiWindow;
    };

} // namespace Demo
