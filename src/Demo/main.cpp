#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "Demo00_Application.h"
using Demo::Demo00_Application;

int main(void)
{
    Demo00_Application application;
    if (!application.init())
    {
        PK_LOG_ERRORF("Application failed to initialize.");
        return -1;
    }
    application.run();

    return 0;
}
