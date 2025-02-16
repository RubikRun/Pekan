#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "DemoApplication.h"
using Demo::DemoApplication;

#include "PekanRenderer.h"
using Pekan::Renderer::PekanRenderer;

int main(void)
{
    // TEMP: Just to see if PekanRenderer is linked properly
    PekanRenderer renderer;
    renderer.test();

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
