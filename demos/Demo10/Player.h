#pragma once

#include "Scene.h"
#include "TransformComponent2D.h"
#include "Demo10_GUIWindow.h"
#include <entt/entt.hpp>

namespace Demo
{

    class Player
    {
    public:

        void create(Pekan::Scene* scene);
        void destroy();

        void update(Demo10_GUIWindow* guiWindow);

    private:

        entt::entity m_body = entt::null;
        entt::entity m_leftArmJoint = entt::null;
        entt::entity m_leftArm = entt::null;
        entt::entity m_rightArmJoint = entt::null;
        entt::entity m_rightArm = entt::null;
        entt::entity m_leftLegJoint = entt::null;
        entt::entity m_leftLeg = entt::null;
        entt::entity m_rightLegJoint = entt::null;
        entt::entity m_rightLeg = entt::null;
        entt::entity m_head = entt::null;
        entt::entity m_sword = entt::null;

        Pekan::Scene* m_scene = nullptr;
    };

} // namespace Demo
