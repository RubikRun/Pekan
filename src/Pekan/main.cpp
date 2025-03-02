#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;

const char* glsl_version = "#version 330 core";

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

GLFWwindow* setupWindow()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "ERROR: GLFW failed to initialize." << std::endl;
        return nullptr;
    }

    // Set window hint for OpenGL 4.3 and OpenGL Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pekan v0.0", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "ERROR: Failed to create a window with GLFW." << std::endl;
        glfwTerminate();
        return nullptr;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Enalbe VSync
    glfwSwapInterval(1);

    return window;
}

void cleanupWindow(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool loadOpenGL()
{
    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: GLAD failed to load OpenGL functions." << std::endl;
        return false;
    }
    // Set OpenGL viewport's resolution to be the same as window's resolution
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Log OpenGL version
    std::cout << "Successfully loaded OpenGL " << glGetString(GL_VERSION) << std::endl;

    return true;
}

void setupImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

bool setupOpenGLDemo(unsigned& shaderProgram, unsigned& vao, unsigned& vbo, unsigned& ebo)
{
    // Compile vertex shader
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Vertex shader failed to compile with the following error: " << infoLog << std::endl;
        return false;
    }
    // Compile fragment shader
    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Fragment shader failed to compile with the following error: " << infoLog << std::endl;
        return false;
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR: Shader program failed to link with the following error: " << infoLog << std::endl;
        return false;
    }
    // Delete vertex shader and fragment shader, as they are already linked into a shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and configure vertex attributes
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    // Generate a vertex array object, a vertex buffer object, an element buffer object
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // Bind vertex array object
    glBindVertexArray(vao);
    // Bind vertex buffer object and fill with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind element buffer object and fill with indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

void renderOpenGLDemo(const unsigned& shaderProgram, const unsigned& vao)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cleanupOpenGLDemo(const unsigned& shaderProgram, const unsigned& vao, const unsigned& vbo, const unsigned& ebo)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);
}

void drawImGuiDemo(GLFWwindow* window, ImVec4& clearColor)
{
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(200, 70));
    ImGui::Begin("Pekan");

    ImGui::Text("Background Color");
    ImGui::ColorEdit3("", (float*)&clearColor);

    ImGui::End();

    ImGui::Render();
}

void renderImGuiDemo()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void cleanupImGuiDemo()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main(void)
{
    GLFWwindow* window = setupWindow();
    if (window == nullptr)
    {
        return -1;
    }
    if (!loadOpenGL())
    {
        return -1;
    }
    setupImGui(window);

    unsigned shaderProgram, vao, vbo, ebo;
    if (!setupOpenGLDemo(shaderProgram, vao, vbo, ebo))
    {
        return -1;
    }

    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        drawImGuiDemo(window, clearColor);
        
        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        renderOpenGLDemo(shaderProgram, vao);
        renderImGuiDemo();

        glfwSwapBuffers(window);
    }

    cleanupOpenGLDemo(shaderProgram, vao, vbo, ebo);
    cleanupImGuiDemo();

    cleanupWindow(window);
    return 0;
}