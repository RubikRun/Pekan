#define PK_FILENAME "main.cpp"
#include "Logger/PekanLogger.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        PK_LOG_ERRORF("ERROR: Vertex shader failed to compile with the following error: " << infoLog);
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
        PK_LOG_ERRORF("ERROR: Fragment shader failed to compile with the following error: " << infoLog);
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
        PK_LOG_ERRORF("ERROR: Shader program failed to link with the following error: " << infoLog);
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

int main(void)
{
    PekanEngine engine;
    engine.init();
    GLFWwindow* window = engine.getWindow();

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
    engine.exit();

    return 0;
}