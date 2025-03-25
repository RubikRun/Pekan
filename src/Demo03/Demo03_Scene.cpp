#include "Demo03_Scene.h"

#include "Utils/PekanUtils.h"

using Pekan::PekanEngine;
using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Utils::getRandomFloat;

static const char* apple_vertexShaderFilePath = "resources/03_apple_vertexShader.glsl";
static const char* apple_fragmentShaderFilePath = "resources/03_apple_fragmentShader.glsl";
static const float APPLE_SIZE = 0.03f;

namespace Demo
{

	bool Demo03_Scene::init()
	{
		m_aspectRatio = float(PekanEngine::getWindowWidth()) / float(PekanEngine::getWindowHeight());

		m_snake.init();

		// Generate a random apple position
		m_applePosition =
		{
			getRandomFloat(-1.0f, 1.0f - APPLE_SIZE),
			getRandomFloat(-1.0f, 1.0f - APPLE_SIZE)
		};

		const float appleVertices[] =
		{
			m_applePosition.x, m_applePosition.y,
			m_applePosition.x, m_applePosition.y + APPLE_SIZE * m_aspectRatio,
			m_applePosition.x + APPLE_SIZE, m_applePosition.y + APPLE_SIZE * m_aspectRatio,
			m_applePosition.x + APPLE_SIZE, m_applePosition.y
		};

		const int appleIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		m_apple.create
		(
			appleVertices,
			8 * sizeof(float),
			BufferDataUsage::DynamicDraw,
			{ { ShaderDataType::Float2, "position" } },
			appleIndices,
			6 * sizeof(int),
			BufferDataUsage::StaticDraw,
			Pekan::Utils::readFileToString(apple_vertexShaderFilePath).c_str(),
			Pekan::Utils::readFileToString(apple_fragmentShaderFilePath).c_str()
		);

        return true;
	}

	void Demo03_Scene::update()
	{
		m_snake.update();
	}

	void Demo03_Scene::render()
	{
		PekanRenderer::clear();
		// Render apple
		m_apple.bind();
		PekanRenderer::drawIndexed(6);
		m_apple.unbind();
		// Render snake
		m_snake.render();
	}

	void Demo03_Scene::exit()
	{
		m_snake.exit();
		m_apple.destroy();
	}

} // namespace Demo