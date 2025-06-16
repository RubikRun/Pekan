#include "Logger/PekanLogger.h"

#include "Demo07_Application.h"
using Demo::Demo07_Application;

int main(void)
{
    Demo07_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
