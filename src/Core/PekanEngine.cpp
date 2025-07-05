#include "PekanEngine.h"

#include "Logger/PekanLogger.h"
#include "PekanApplication.h"
#include "SubsystemManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define PK_GLSL_VERSION "#version 330 core"

namespace Pekan
{
    const int DEFAULT_WINDOW_WIDTH = 1280;
    const int DEFAULT_WINDOW_HEIGHT = 720;

    const char* DEFAULT_WINDOW_TITLE = "Pekan v0.1";

    Window PekanEngine::s_window;
    PekanApplication* PekanEngine::s_application = nullptr;
    bool PekanEngine::isInitialized = false;
    bool PekanEngine::isWindowCreated = false;

    // Returns a user-friendly string from given OpenGL error code
    std::string _getGLErrorMessage(unsigned error)
    {
        switch (error) {
            case GL_NO_ERROR:                         return "No error";
            case GL_INVALID_ENUM:                     return "Invalid enum";
            case GL_INVALID_VALUE:                    return "Invalid value";
            case GL_INVALID_OPERATION:                return "Invalid operation";
            case GL_STACK_OVERFLOW:                   return "Stack overflow";
            case GL_STACK_UNDERFLOW:                  return "Stack underflow";
            case GL_OUT_OF_MEMORY:                    return "Out of memory";
            case GL_INVALID_FRAMEBUFFER_OPERATION:    return "Invalid framebuffer operation";
        }
        return "Unknown error";
    }

#if PK_OPENGL_VERSION_MAJOR >= 4 && PK_OPENGL_VERSION_MINOR >= 3

    // A callback function that will be called by OpenGL every time there is an error (or other) message.
    static void APIENTRY openGLDebugCallback(
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

    bool PekanEngine::init(PekanApplication* application)
    {
        if (s_application != nullptr)
        {
            PK_LOG_ERROR("Multiple applications are trying to initialize the engine. Pekan supports only one application at a time.", "Pekan");
            return false;
        }
        if (application == nullptr)
        {
            PK_LOG_ERROR("A null application is given when trying to initialize the engine.", "Pekan");
            return false;
        }
        s_application = application;

        // TODO: we need to call
        //     SubsystemManager::initAll();
        // here, but currently we can't because to initialize Renderer2D we need a valid OpenGL context
        // which we receive when creating the window, but currently creating the window is done in application code,
        // specifically in the _init() function of each application.

        isInitialized = true;
        return true;
    }

    void PekanEngine::exit()
    {
        if (!isInitialized)
        {
            PK_LOG_ERROR("Trying to exit engine but engine is not yet initialized.", "Pekan");
            return;
        }
        if (isWindowCreated)
        {
            exitImGui();
            s_window.destroy();
            isWindowCreated = false;
        }

        // TODO: we need to call
        //     SubsystemManager::exitAll();
        // here, but currently we can't. See explanation above in init() function.

        isInitialized = false;
    }

    bool PekanEngine::createWindow(WindowProperties properties)
    {
        if (!s_window.create(properties))
        {
            return false;
        }
        if (!loadOpenGL())
        {
            return false;
        }
        if (!initImGui())
        {
            return false;
        }

        isWindowCreated = true;
        return true;
    }

    bool PekanEngine::isKeyPressed(KeyCode key)
    {
        return s_window.isKeyPressed(key);
    }

    bool PekanEngine::isKeyReleased(KeyCode key)
    {
        return s_window.isKeyReleased(key);
    }

    bool PekanEngine::isKeyRepeating(KeyCode key)
    {
        return s_window.isKeyRepeating(key);
    }

    glm::vec2 PekanEngine::getMousePosition()
    {
        return s_window.getMousePosition();
    }

    bool PekanEngine::isMouseButtonPressed(MouseButton button)
    {
        return s_window.isMouseButtonPressed(button);
    }

    bool PekanEngine::isMouseButtonReleased(MouseButton button)
    {
        return s_window.isMouseButtonReleased(button);
    }

    glm::ivec2 PekanEngine::getWindowSize()
    {
        return s_window.getSize();
    }

    bool PekanEngine::loadOpenGL()
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
        glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

        // Log OpenGL version
        PK_LOG_INFO("Successfully loaded OpenGL " << glGetString(GL_VERSION), "Pekan");

        return true;
    }

    bool PekanEngine::initImGui()
    {
        if (!isInitialized)
        {
            PK_LOG_ERROR("Trying to initialize ImGui but engine is not yet initialized.", "Pekan");
            return false;
        }
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Setup ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(s_window.m_glfwWindow, true);
        ImGui_ImplOpenGL3_Init(PK_GLSL_VERSION);

        // Scale up widgets and font by a factor of 2
        style.ScaleAllSizes(1.3f);
        io.FontGlobalScale = 1.05f;

        return true;
    }

    void PekanEngine::exitImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace Pekan