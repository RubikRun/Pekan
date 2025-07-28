#include "GraphicsSystem.h"
#include "GUISystem.h"

#include "PekanLogger.h"

#include "Demo02_Application.h"
using Demo::Demo02_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_GUI;

    Demo02_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
