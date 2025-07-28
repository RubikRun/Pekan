#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace GUI
{

// Use this macro in your application's main function to include the GUI subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_GUI Pekan::GUI::GUISystem::registerSubsystem()

	class GUISystem : public ISubsystem
	{
	public:

		std::string getSubsystemName() const override { return "GUI"; }

		// Registers GUI as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerSubsystem();

		// Returns a pointer to the global GUI instance
		static GUISystem* getInstance();

		// Generates a new ID for a GUI widget
		static int generateWidgetId();

	private: /* functions */

		bool init() override;
		void exit() override;

		// Initializes ImGui library, and creates ImGui context
		static bool initImGui();
		// Exists ImGui library, and destroys ImGui context
		static void exitImGui();

	private: /* variables */

		// Next available ID for a GUI widget
		static int m_nextWidgetId;
	};

} // namespace GUI
} // namespace Pekan
