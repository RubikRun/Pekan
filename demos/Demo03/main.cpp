#include "GraphicsSubsystem.h"

#include "PekanLogger.h"

#include "Demo03_Application.h"
using Demo::Demo03_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;

    Demo03_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
