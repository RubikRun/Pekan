#include "Logger/PekanLogger.h"

#include "Demo01_Application.h"
using Demo::Demo01_Application;

int main(void)
{
    Demo01_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.");
        return -1;
    }
    application.run();

    return 0;
}
