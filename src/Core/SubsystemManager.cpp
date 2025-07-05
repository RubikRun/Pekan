#include "SubsystemManager.h"

namespace Pekan
{
	static std::vector<ISubsystem*> g_subsystems;

	void SubsystemManager::registerSubsystem(ISubsystem* subsystem)
	{
		g_subsystems.push_back(subsystem);
	}

	void SubsystemManager::initAll()
	{
		for (ISubsystem* subsystem : g_subsystems)
		{
			subsystem->init();
		}
	}

	void SubsystemManager::exitAll()
	{
		for (auto it = g_subsystems.rbegin(); it != g_subsystems.rend(); ++it)
		{
			(*it)->exit();
		}
	}
}
