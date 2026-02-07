#pragma once

#include "ISubsystem.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

// Use this macro in your application's main function to include the Renderer2D subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D Pekan::Renderer2D::Renderer2DSubsystem::registerAsSubsystem()

	class Renderer2DSubsystem : public ISubsystem
	{

	public:

		std::string getSubsystemName() const override { return "Renderer2D"; }

		// Registers Renderer2DSubsystem as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerAsSubsystem();

		// To be called at the beginning of every 2D scene's render() function.
		static void beginFrame();
		// To be called at the end of every 2D scene's render() function.
		static void endFrame();

		// TODO: this function has to be renamed, removing the "_ECS" suffix, once ECS is fully integrated
		// Returns current mouse position in world space, using scene's primary camera
		static glm::vec2 getMousePosition_ECS(const entt::registry& registry);

	private: /* functions */

		bool init() override;
		void exit() override;

		ISubsystem* getParent() override;
	};

} // namespace Renderer2D
} // namespace Pekan