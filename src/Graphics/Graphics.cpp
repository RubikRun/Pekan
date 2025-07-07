#include "Graphics.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
namespace Graphics
{

#if PK_OPENGL_VERSION_MAJOR >= 4 && PK_OPENGL_VERSION_MINOR >= 3
	static void enableOpenGLDebugOutput();
#endif

	static Graphics g_graphics;

	void Graphics::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_graphics);
	}

	Graphics* Graphics::getInstance()
	{
		return &g_graphics;
	}

	bool Graphics::loadOpenGL()
	{
		// Load OpenGL function pointers with GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			PK_LOG_ERROR("Failed to load OpenGL function pointers with GLAD", "Pekan");
			return false;
		}
#if PK_OPENGL_VERSION_MAJOR >= 4 && PK_OPENGL_VERSION_MINOR >= 3
		// Enable OpenGL's debug output, so that errors and other messages are handled by our callback function openGLDebugCallback()
		enableOpenGLDebugOutput();
#endif
		// Set OpenGL viewport's resolution to be the same as window's resolution
		const glm::ivec2 windowSize = PekanEngine::getWindowSize();
		glViewport(0, 0, windowSize.x, windowSize.y);

		// Log OpenGL version
		PK_LOG_INFO("Successfully loaded OpenGL " << glGetString(GL_VERSION), "Pekan");

		return true;
	}

	void Graphics::init()
	{
		loadOpenGL();
		m_isInitialized = true;
	}

	void Graphics::exit()
	{
		m_isInitialized = false;
	}

#if PK_OPENGL_VERSION_MAJOR >= 4 && PK_OPENGL_VERSION_MINOR >= 3

	// A callback function that will be called by OpenGL every time there is an error (or other) message.
	static void APIENTRY openGLDebugCallback
	(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam
	)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:            PK_LOG_ERROR(message, "OpenGL");      break;
			case GL_DEBUG_SEVERITY_MEDIUM:          PK_LOG_WARNING(message, "OpenGL");    break;
			case GL_DEBUG_SEVERITY_LOW:             PK_LOG_INFO(message, "OpenGL");       break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:    PK_LOG_DEBUG(message, "OpenGL");      break;
		}
	}

	// Enables OpenGL's debug output and binds our callback function
	static void enableOpenGLDebugOutput()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		// Register the callback function
		glDebugMessageCallback(openGLDebugCallback, nullptr);
	}

#endif

} // namespace Graphics
} // namespace Pekan