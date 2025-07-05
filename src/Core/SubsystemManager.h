#pragma once
#include <vector>
#include "ISubsystem.h"

namespace Pekan
{

    class SubsystemManager
    {
    public:

        // Registers a subsystem in Pekan
        static void registerSubsystem(ISubsystem* subsystem);

        // Initializes all registered subsystems, in order of registration
        static void initAll();
        // Exits all registered subsystems, in reverse order of registration
        static void exitAll();
    };

}