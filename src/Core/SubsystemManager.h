#pragma once
#include "ISubsystem.h"

namespace Pekan
{

    // A static class used for managing Pekan's subsystems
    class SubsystemManager
    {
        friend class PekanEngine;

    public:

        // Registers a subsystem in Pekan
        static void registerSubsystem(ISubsystem* subsystem);

    private:

        // Initializes a given subsystem,
        // recursively making sure that parent is initialized first.
        static void initSubsystem(ISubsystem* subsystem);

        // Initializes all registered subsystems in order from parents to leafs
        static void initAll();
        // Exits all registered subsystems
        static void exitAll();
    };

}