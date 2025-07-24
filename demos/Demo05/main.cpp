#include "Graphics.h"
#include "Renderer2D.h"
#include "GUI.h"

#include "PekanLogger.h"

#include "Demo05_Application.h"
using Demo::Demo05_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;
    PEKAN_INCLUDE_SUBSYSTEM_GUI;

    Demo05_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
