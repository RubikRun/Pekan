#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace GUI
{

// Use this macro in your application's main function to include the GUI subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_GUI Pekan::GUI::GUI::registerSubsystem()

	class GUI : public ISubsystem
	{
	public:

		// Registers GUI as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerSubsystem();

		// Returns a pointer to the global GUI instance
		static GUI* getInstance();

	private: /* functions */

		void init() override;
		void exit() override;

		bool isInitialized() override { return m_isInitialized; }

	private: /* variables */

		bool m_isInitialized = false;
	};

} // namespace GUI
} // namespace Pekan
