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
		PK_ASSERT_QUICK(subsystem != nullptr);
		if (subsystem->m_isInitialized)
		{
			return;
		}
		// If subsystem has a parent that is not yet initialized, initialize the parent first
		ISubsystem* parent = subsystem->getParent();
		if (parent != nullptr && !parent->m_isInitialized)
		{
			initSubsystem(parent);
		}
		// Initialize subsystem
		subsystem->m_isInitialized = subsystem->init();
		if (!subsystem->m_isInitialized)
		{
			PK_LOG_ERROR("Failed to initialize subsystem \"" << subsystem->getSubsystemName() << "\".", "Pekan");
		}
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
		for (ISubsystem* subsystem : g_subsystems)
		{
			PK_ASSERT(subsystem->m_isInitialized, "Trying to exit a subsystem that is not yet initialized.", "Pekan");
			subsystem->exit();
			subsystem->m_isInitialized = false;
		}
	}
}
