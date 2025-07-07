#include "Graphics.h"

#include "PekanLogger.h"

#include "Demo04_Application.h"
using Demo::Demo04_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;

    Demo04_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
