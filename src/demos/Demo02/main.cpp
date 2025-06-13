#include "Logger/PekanLogger.h"

#include "Demo02_Application.h"
using Demo::Demo02_Application;

int main(void)
{
    Demo02_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
