#include "GraphicsSystem.h"
#include "Renderer2DSystem.h"

#include "PekanLogger.h"

#include "DemoIg00_Application.h"
using Demo::DemoIg00_Application;

int main(void)
{
	PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
	PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;

	DemoIg00_Application application;
	if (!application.init())
	{
		PK_LOG_ERROR("Application failed to initialize.", "Pekan");
		return -1;
	}
	application.run();

	return 0;
}
