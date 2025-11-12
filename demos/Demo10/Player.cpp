#include "Player.h"

#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "SolidColorMaterialComponent.h"
#include "Image.h"
#include "Texture2D.h"
#include "PekanLogger.h"

using namespace Pekan;
using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;

#define ARM_IMAGE_FILEPATH "assets/arm.png"
#define LEG_IMAGE_FILEPATH "assets/leg.png"
#define HEAD_IMAGE_FILEPATH "assets/head.png"
#define SWORD_IMAGE_FILEPATH "assets/sword.png"

namespace Demo
{

    void Player::create(Scene* scene)
    {
        PK_ASSERT_QUICK(scene != nullptr);

        m_scene = scene;
        entt::registry& registry = m_scene->getRegistry();

        // Create body
        {
            m_body = m_scene->createEntity();
            registry.emplace<TransformComponent2D>(m_body);
            registry.emplace<RectangleGeometryComponent>(m_body, 1.0f, 5.0f);
            registry.emplace<SolidColorMaterialComponent>(m_body, glm::vec4{ 0.29f, 0.15f, 0.51f, 1.0f });
        }
        // Create left arm joint
        {
            m_leftArmJoint = m_scene->createEntity();
            TransformComponent2D transform;
            transform.position = glm::vec2(-0.5f, 0.5f);
            transform.parent = m_body;
            registry.emplace<TransformComponent2D>(m_leftArmJoint, transform);
        }
        // Create left arm
        {
            m_leftArm = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_leftArmJoint;
            registry.emplace<TransformComponent2D>(m_leftArm, transform);
            SpriteComponent sprite;
            sprite.width = 2.868995f;
            sprite.height = 1.0f;
            // Set sprite's texture
            {
                Image image(ARM_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_leftArm, sprite);
        }
        // Create right arm joint
        {
            m_rightArmJoint = m_scene->createEntity();
            TransformComponent2D transform;
            transform.position = glm::vec2(0.5f, 0.5f);
            transform.parent = m_body;
            registry.emplace<TransformComponent2D>(m_rightArmJoint, transform);
        }
        // Create right arm
        {
            m_rightArm = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_rightArmJoint;
            registry.emplace<TransformComponent2D>(m_rightArm, transform);
            SpriteComponent sprite;
            sprite.width = 2.868995f;
            sprite.height = 1.0f;
            // Set sprite's texture
            {
                Image image(ARM_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_rightArm, sprite);
        }
        // Create left leg joint
        {
            m_leftLegJoint = m_scene->createEntity();
            TransformComponent2D transform;
            transform.position = glm::vec2(-0.5f, -2.5f);
            transform.parent = m_body;
            registry.emplace<TransformComponent2D>(m_leftLegJoint, transform);
        }
        // Create left leg
        {
            m_leftLeg = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_leftLegJoint;
            registry.emplace<TransformComponent2D>(m_leftLeg, transform);
            SpriteComponent sprite;
            sprite.width = 1.0;
            sprite.height = 1.2553f;
            // Set sprite's texture
            {
                Image image(LEG_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_leftLeg, sprite);
        }
        // Create right leg joint
        {
            m_rightLegJoint = m_scene->createEntity();
            TransformComponent2D transform;
            transform.position = glm::vec2(0.5f, -2.5f);
            transform.parent = m_body;
            registry.emplace<TransformComponent2D>(m_rightLegJoint, transform);
        }
        // Create right leg
        {
            m_rightLeg = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_rightLegJoint;
            registry.emplace<TransformComponent2D>(m_rightLeg, transform);
            SpriteComponent sprite;
            sprite.width = 1.0;
            sprite.height = 1.2553f;
            // Set sprite's texture
            {
                Image image(LEG_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_rightLeg, sprite);
        }
        // Create head
        {
            m_head = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_body;
            registry.emplace<TransformComponent2D>(m_head, transform);
            SpriteComponent sprite;
            sprite.width = 1.0f;
            sprite.height = 1.3754f;
            // Set sprite's texture
            {
                Image image(HEAD_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_head, sprite);
        }
        // Create sword
        {
            m_sword = m_scene->createEntity();
            TransformComponent2D transform;
            transform.parent = m_rightArm;
            registry.emplace<TransformComponent2D>(m_sword, transform);
            SpriteComponent sprite;
            sprite.width = 1.0f;
            sprite.height = 1.0f;
            // Set sprite's texture
            {
                Image image(SWORD_IMAGE_FILEPATH);
                Texture2D_Ptr texture = std::make_shared<Texture2D>();
                texture->create(image);
                sprite.texture = texture;
            }
            registry.emplace<SpriteComponent>(m_sword, sprite);
        }
    }

    void Player::destroy()
    {
        PK_ASSERT_QUICK(m_scene != nullptr);

        m_scene->destroyEntity(m_body);
    }

    void Player::update(Demo10_GUIWindow* guiWindow)
    {
        PK_ASSERT_QUICK(m_scene != nullptr);
        PK_ASSERT_QUICK(guiWindow != nullptr);
        entt::registry& registry = m_scene->getRegistry();

        // Update player body transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_body);
            transform.position = guiWindow->getBodyPosition();
            transform.rotation = guiWindow->getBodyRotation();
            transform.scale = guiWindow->getBodyScale();
        }
        // Update player left arm transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_leftArm);
            transform.position = guiWindow->getLeftArmPosition();
            transform.scale = guiWindow->getLeftArmScale();
            TransformComponent2D& jointTransform = registry.get<TransformComponent2D>(m_leftArmJoint);
            jointTransform.rotation = guiWindow->getLeftArmRotation();
        }
        // Update player right arm transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_rightArm);
            transform.position = guiWindow->getRightArmPosition();
            transform.scale = guiWindow->getRightArmScale();
            TransformComponent2D& jointTransform = registry.get<TransformComponent2D>(m_rightArmJoint);
            jointTransform.rotation = guiWindow->getRightArmRotation();
        }
        // Update player left leg transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_leftLeg);
            transform.position = guiWindow->getLeftLegPosition();
            transform.scale = guiWindow->getLeftLegScale();
            TransformComponent2D& jointTransform = registry.get<TransformComponent2D>(m_leftLegJoint);
            jointTransform.rotation = guiWindow->getLeftLegRotation();
        }
        // Update player right leg transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_rightLeg);
            transform.position = guiWindow->getRightLegPosition();
            transform.scale = guiWindow->getRightLegScale();
            TransformComponent2D& jointTransform = registry.get<TransformComponent2D>(m_rightLegJoint);
            jointTransform.rotation = guiWindow->getRightLegRotation();
        }
        // Update player head transfomr from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_head);
            transform.position = guiWindow->getHeadPosition();
            transform.rotation = guiWindow->getHeadRotation();
            transform.scale = guiWindow->getHeadScale();
        }
        // Update player sword transform from GUI window
        {
            TransformComponent2D& transform = registry.get<TransformComponent2D>(m_sword);
            transform.position = guiWindow->getSwordPosition();
            transform.rotation = guiWindow->getSwordRotation();
            transform.scale = guiWindow->getSwordScale();
        }
    }

} // namespace Demo
