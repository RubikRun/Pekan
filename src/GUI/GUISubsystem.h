#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace GUI
{

// Use this macro in your application's main function to include the GUI subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_GUI Pekan::GUI::GUISubsystem::registerAsSubsystem()

	class GUISubsystem : public ISubsystem
	{
	public:

		std::string getSubsystemName() const override { return "GUI"; }

		// Registers GUI as a subsystem in Pekan's SubsystemManager,
		// so that it's automatically initialized and exited.
		static void registerAsSubsystem();

		// Returns a pointer to the global GUI instance
		static GUISubsystem* getInstance();

		// Generates a new ID for a GUI widget
		static int generateWidgetId();

	private: /* functions */

		// Begins a new ImGui frame (called automatically at the start of each frame)
		static void beginFrame();
		// Ends the current ImGui frame and renders it (called automatically at the end of each frame)
		static void endFrame();

		bool init() override;
		void exit() override;

		// Initializes ImGui library, and creates ImGui context
		static bool initImGui();
		// Exists ImGui library, and destroys ImGui context
		static void exitImGui();

	private: /* variables */

		// Next available ID for a GUI widget
		inline static int m_nextWidgetId = 0;

		// A flag indicating if an ImGui frame is currently active
		inline static bool m_isFrameActive = false;
	};

} // namespace GUI
} // namespace Pekan
