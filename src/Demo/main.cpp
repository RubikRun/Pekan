#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "DemoApplication.h"
using Demo::DemoApplication;

int main(void)
{
    DemoApplication application;
    if (!application.init())
    {
        PK_LOG_ERRORF("Application failed to initialize.");
        return -1;
    }
    application.run();
    application.exit();

    return 0;
}
