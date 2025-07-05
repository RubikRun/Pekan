#include "SubsystemManager.h"

#include "PekanLogger.h"

namespace Pekan
{
	static std::vector<ISubsystem*> g_subsystems;

	void SubsystemManager::registerSubsystem(ISubsystem* subsystem)
	{
		if (subsystem == nullptr)
		{
			PK_LOG_ERROR("Cannot register a NULL subsystem in Pekan.", "Pekan");
			return;
		}
		g_subsystems.push_back(subsystem);
	}

	void SubsystemManager::initSubsystem(ISubsystem* subsystem)
	{
		if (subsystem->isInitialized())
		{
			return;
		}
		// If subsystem has a parent that is not yet initialized, initialize the parent first.
		ISubsystem* parent = subsystem->getParent();
		if (parent != nullptr && !parent->isInitialized())
		{
			initSubsystem(parent);
		}
		// Initialize subsystem
		subsystem->init();
	}

	void SubsystemManager::initAll()
	{
		for (ISubsystem* subsystem : g_subsystems)
		{
			initSubsystem(subsystem);
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
