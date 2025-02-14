#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;

#include "Demo/DemoApplication.h"
using Pekan::DemoApplication;

int main(void)
{
    PekanEngine engine;
    if (!engine.init())
    {
        PK_LOG_ERRORF("Engine failed to initialize.");
        return -1;
    }

    GLFWwindow* window = engine.getWindow();
    const int width = engine.getWindowWidth();
    const int height = engine.getWindowHeight();

    DemoApplication application;
    if (!application.init(window, width, height))
    {
        PK_LOG_ERRORF("Application failed to initialize.");
        return -1;
    }
    application.run();

    engine.exit();

    return 0;
}
