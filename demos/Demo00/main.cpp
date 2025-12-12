#include "GraphicsSubsystem.h"
#include "GUISubsystem.h"

#include "PekanLogger.h"

#include "Demo00_Application.h"
using Demo::Demo00_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_GUI;

    Demo00_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
