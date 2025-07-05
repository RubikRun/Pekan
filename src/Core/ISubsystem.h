#pragma once

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
	private:
		virtual void init() = 0;
		virtual void exit() = 0;
	};

} // namespace Pekan
