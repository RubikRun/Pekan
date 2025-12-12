#include "GraphicsSubsystem.h"
#include "Renderer2DSubsystem.h"
#include "GUISubsystem.h"

#include "PekanLogger.h"

#include "Demo08_Application.h"
using Demo::Demo08_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;
    PEKAN_INCLUDE_SUBSYSTEM_GUI;

    Demo08_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
