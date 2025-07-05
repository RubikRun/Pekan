#include "PekanLogger.h"

#include "Demo05_Application.h"
using Demo::Demo05_Application;

int main(void)
{
    Demo05_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
