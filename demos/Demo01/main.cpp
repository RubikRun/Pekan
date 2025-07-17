#include "Graphics.h"
#include "GUI.h"

#include "PekanLogger.h"

#include "Demo01_Application.h"
using Demo::Demo01_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_GUI;

    Demo01_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
