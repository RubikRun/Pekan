#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace Graphics
{

// Use this macro in your application's main function to include the Graphics subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS Pekan::Graphics::Graphics::registerSubsystem()

	class Graphics : public ISubsystem
	{
	public:

		// Registers Graphics as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerSubsystem();

		// Returns a pointer to the global Graphics instance
		static Graphics* getInstance();

		// Loads OpenGL function pointers
		static bool loadOpenGL();

	private: /* functions */

		void init() override;
		void exit() override;

		bool isInitialized() override { return m_isInitialized; }

	private: /* variables */

		bool m_isInitialized = false;
	};

} // namespace Graphics
} // namespace Pekan
