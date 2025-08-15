#pragma once

#include <string>

namespace Pekan
{

	class SubsystemManager;

	// An interface for Pekan's subsystems.
	// A subsystem is initialized when Pekan is initialized, and exited when Pekan is exited.
	class ISubsystem
	{
		friend class SubsystemManager;

	public:

		virtual ~ISubsystem() = default;

		virtual std::string getSubsystemName() const = 0;

	private:

		virtual bool init() = 0;
		virtual void exit() = 0;

		// Can be overridden by derived classes to return subsystem's parent.
		// A subsystem depends on its parent, meaning that its parent must be initialized first.
		// If a NULL parent is returned, subsystem's parent will be Core.
		virtual ISubsystem* getParent() { return nullptr; }

		// A flag indicating if subsystem has been initialized and not yet exited
		bool m_isInitialized = false;
	};

} // namespace Pekan
