#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;

#include "DemoApplication.h"
using Demo::DemoApplication;

int main(void)
{
    if (!PekanEngine::init())
    {
        PK_LOG_ERRORF("Engine failed to initialize.");
        return -1;
    }

    GLFWwindow* window = PekanEngine::getWindow();
    const int width = PekanEngine::getWindowWidth();
    const int height = PekanEngine::getWindowHeight();

    DemoApplication application;
    if (!application.init(window, width, height))
    {
        PK_LOG_ERRORF("Application failed to initialize.");
        return -1;
    }
    application.run();

    PekanEngine::exit();

    return 0;
}
