#include "PekanLogger.h"

#include "Demo06_Application.h"
using Demo::Demo06_Application;

int main(void)
{
    Demo06_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();

    return 0;
}
