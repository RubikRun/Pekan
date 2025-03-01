#include "Demo01_Scene.h"
#define PK_FILENAME "DemoScene.cpp"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

#include <glad/glad.h>

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::VertexBufferElement;
using Pekan::Renderer::VertexBufferLayout;
using Pekan::Renderer::VertexBufferDataUsage;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;

#include <GLFW/glfw3.h>

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/01_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/01_fragment_shader.glsl";

namespace Demo
{

	void Demo01_Scene::update()
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
                // bottom left
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom right
                (x + std::cos(angle - PI / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle - PI / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom left
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top left
                (x + std::cos(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(width) - 1.0f,
                (y + std::sin(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
            };

            vertices.insert(vertices.end(), std::begin(squareVertices), std::end(squareVertices));
        }

        vertexArray.bind();
        vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(float), VertexBufferDataUsage::DynamicDraw);
	}

	void Demo01_Scene::render()
	{
        // If there is no GUI window attached, just clear background with default color
        PekanRenderer::clear();

        shader.bind();
        vertexArray.bind();
        PekanRenderer::draw(vertices.size() / 6);
        vertexArray.unbind();
        shader.unbind();
	}

	void Demo01_Scene::exit()
	{
        vertexBuffer.destroy();
        vertexArray.destroy();
        shader.destroy();
	}

    void Demo01_Scene::addSquare()
    {
        Rectangle newSquare;
        newSquare.width = 60;
        newSquare.height = 60;
        newSquare.x = width / 2;
        newSquare.y = height / 2;
        newSquare.id = squares.size();
        squares.push_back(newSquare);
    }

	bool Demo01_Scene::_init()
	{
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader.create(
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        // Create a vertex array
        vertexArray.create();
        // Create an empty vertex buffer, with layout specified, and add it to vertex array
        vertexBuffer.create();
        vertexArray.addVertexBuffer(vertexBuffer, { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });

        return true;
	}

} // namespace Demo