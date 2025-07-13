#include "Graphics.h"
#include "Renderer2D.h"

#include "PekanLogger.h"

#include "Demo04_Application.h"
using Demo::Demo04_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;

    Demo04_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
