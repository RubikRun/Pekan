#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace Graphics
{

// Use this macro in your application's main function to include the Graphics subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS Pekan::Graphics::GraphicsSubsystem::registerAsSubsystem()

	class GraphicsSubsystem : public ISubsystem
	{
	public:

		std::string getSubsystemName() const override { return "Graphics"; }

		// Registers GraphicsSubsystem as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerAsSubsystem();

		// Returns a pointer to the global GraphicsSubsystem instance
		static GraphicsSubsystem* getInstance();

		// Loads OpenGL function pointers
		static bool loadOpenGL();

	private: /* functions */

		bool init() override;
		void exit() override;
	};

} // namespace Graphics
} // namespace Pekan
