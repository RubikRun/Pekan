#include "SquaresScene.h"

#include <glad/glad.h>

#include <iostream>

#include <ctime>
#include <cmath>

#define PI 3.14159265359f

namespace Pekan
{

    static const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    static const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

	bool SquaresScene::init(int windowWidth, int windowHeight)
	{
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;

        // Compile vertex shader
        unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        // Check for shader compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cout << "ERROR in SquaresScene: Vertex shader failed to compile with the following error: " << infoLog << std::endl;
            return false;
        }
        // Compile fragment shader
        unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        // Check for shader compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "ERROR in SquaresScene: Fragment shader failed to compile with the following error: " << infoLog << std::endl;
            return false;
        }
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "ERROR in SquaresScene: Shader program failed to link with the following error: " << infoLog << std::endl;
            return false;
        }
        // Delete vertex shader and fragment shader, as they are already linked into a shader program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Generate a vertex array object, and a vertex buffer object
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        return true;
	}

    void SquaresScene::update()
    {
        vertices.clear();
        for (const Rectangle& square : squares)
        {
            const float x = float(square.x);
            const float y = float(square.y);
            const float w = float(square.width);
            const float h = float(square.height);
            const float angle = float(square.rotation) * PI / 180.0f;
            const float radius = std::sqrtf((w / 2.0f) * (w / 2.0f) + (h / 2.0f) * (h / 2.0f));

            const float squareVertices[] =
            {
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,    // bottom left
                (x + std::cos(angle - PI / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle - PI / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,           // bottom right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,           // top right

                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,    // bottom left
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,           // top right
                (x + std::cos(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowWidth) - 1.0f,
                (y + std::sin(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(windowHeight) - 1.0f,    // top left
            };

            vertices.insert(vertices.end(), std::begin(squareVertices), std::end(squareVertices));
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void SquaresScene::render()
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
        glBindVertexArray(0);
    }

    void SquaresScene::cleanup()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteProgram(shaderProgram);
    }

    void SquaresScene::addSquare() {
        Rectangle newSquare;
        newSquare.width = 60;
        newSquare.height = 60;
        newSquare.x = windowWidth / 2;
        newSquare.y = windowHeight / 2;
        newSquare.id = squares.size();
        squares.push_back(newSquare);
    }

} // namespace Pekan
