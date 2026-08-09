#include "SineGrass.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"

#include <cmath>
#include <algorithm>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;

namespace Demo
{

	bool SineGrass::create(float centerX, float centerY, float width, float height)
	{
		constexpr float PI = 3.14159265f;

		const float xMin = centerX - width * 0.5f;
		const float xMax = centerX + width * 0.5f;
		const float yMin = centerY - height * 0.5f;
		const float yMax = centerY + height * 0.5f;

		m_strands.resize(STRAND_COUNT);
		for (Strand& s : m_strands)
		{
			s.baseX = getRandomFloat(xMin, xMax);
			s.baseY = getRandomFloat(yMin, yMax);
			s.length = getRandomFloat(0.30f, 0.78f);
			s.amplitude = getRandomFloat(0.009f, 0.036f);
			s.thickness = getRandomFloat(0.014f, 0.048f);
			s.oscillations = getRandomFloat(2.0f, 6.0f);
			s.angle = getRandomFloat(-15.0f, 15.0f) * (PI / 180.0f);
			const float g = getRandomFloat(0.48f, 0.62f);
			s.color = { g * 0.42f, g, g * 0.38f };
			s.phase = getRandomFloat(0.0f, 2.0f * PI);
			s.scrollSpeed = getRandomFloat(0.025f, 0.09f);
		}

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_fs.glsl");
		if (vs.empty() || fs.empty())
		{
			PK_LOG_ERROR("Failed to load grass shaders.", "DemoIg00");
			return false;
		}

		m_renderObject.create(
			{
				{ ShaderDataType::Float2, "aPosition" },
				{ ShaderDataType::Float4, "aColor" }
			},
			vs.c_str(),
			fs.c_str()
		);

		const int maxVerts = STRAND_COUNT * (SEGMENTS_PER_STRAND + 1) * 2;
		m_vertexScratch.resize(static_cast<size_t>(maxVerts) * 6);
		m_indexScratch.resize(static_cast<size_t>(STRAND_COUNT * SEGMENTS_PER_STRAND * 6));

		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float)),
			BufferDataUsage::DynamicDraw
		);
		m_renderObject.setIndexData(
			m_indexScratch.data(),
			static_cast<long long>(m_indexScratch.size() * sizeof(unsigned)),
			BufferDataUsage::DynamicDraw
		);

		uploadVertices();
		return true;
	}

	void SineGrass::update(float dt)
	{
		for (Strand& s : m_strands)
		{
			s.phase += s.scrollSpeed * dt;
		}
	}

	void SineGrass::uploadVertices() const
	{
		constexpr float PI = 3.14159265f;

		float* v = m_vertexScratch.data();
		unsigned* idx = m_indexScratch.data();
		unsigned vertBase = 0;
		int indexCount = 0;

		auto emitVert = [&](glm::vec2 pos, const glm::vec3& color)
		{
			*v++ = pos.x;
			*v++ = pos.y;
			*v++ = color.r;
			*v++ = color.g;
			*v++ = color.b;
			*v++ = 1.0f;
		};

		for (const Strand& s : m_strands)
		{
			const float freq = s.oscillations * 2.0f * PI / std::max(s.length, 0.001f);
			const float halfT = s.thickness * 0.5f;
			const float ca = std::cos(s.angle);
			const float sa = std::sin(s.angle);

			auto toWorld = [&](float localX, float localY) -> glm::vec2
			{
				return {
					s.baseX + ca * localX - sa * localY,
					s.baseY + sa * localX + ca * localY
				};
			};

			glm::vec2 prevPos(0.0f);
			for (int i = 0; i <= SEGMENTS_PER_STRAND; i++)
			{
				const float u = static_cast<float>(i) / static_cast<float>(SEGMENTS_PER_STRAND);
				const float t = u * s.length;
				const float localX = s.amplitude * std::sin(freq * (t + s.phase));
				const glm::vec2 pos = toWorld(localX, t);

				glm::vec2 tangent;
				if (i == 0)
				{
					const float t1 = (1.0f / static_cast<float>(SEGMENTS_PER_STRAND)) * s.length;
					const float localX1 = s.amplitude * std::sin(freq * (t1 + s.phase));
					tangent = toWorld(localX1, t1) - pos;
				}
				else
				{
					tangent = pos - prevPos;
				}
				const float len = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
				glm::vec2 perp = { 0.0f, 0.0f };
				if (len > 1e-6f)
				{
					perp = { -tangent.y / len * halfT, tangent.x / len * halfT };
				}
				else
				{
					perp = { halfT, 0.0f };
				}

				emitVert(pos - perp, s.color);
				emitVert(pos + perp, s.color);
				prevPos = pos;
			}

			for (int i = 0; i < SEGMENTS_PER_STRAND; i++)
			{
				const unsigned a = vertBase + static_cast<unsigned>(i) * 2u;
				const unsigned b = a + 1u;
				const unsigned c = a + 2u;
				const unsigned d = a + 3u;
				*idx++ = a; *idx++ = b; *idx++ = c;
				*idx++ = b; *idx++ = d; *idx++ = c;
				indexCount += 6;
			}
			vertBase += static_cast<unsigned>(SEGMENTS_PER_STRAND + 1) * 2u;
		}

		m_indexCount = indexCount;
		const int floatCount = static_cast<int>(vertBase) * 6;
		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(floatCount * sizeof(float))
		);
		m_renderObject.setIndexData(
			m_indexScratch.data(),
			static_cast<long long>(indexCount * sizeof(unsigned))
		);
	}

	void SineGrass::render() const
	{
		if (!m_renderObject.isValid())
		{
			return;
		}

		uploadVertices();
		if (m_indexCount <= 0)
		{
			return;
		}

		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		Shader& shader = m_renderObject.getShader();
		shader.bind();
		if (camera != nullptr)
		{
			shader.setUniformMatrix4fv("uViewProjectionMatrix", camera->getViewProjectionMatrix());
		}
		shader.unbind();

		m_renderObject.render(DrawMode::Triangles);
	}

	void SineGrass::destroy()
	{
		if (m_renderObject.isValid())
		{
			m_renderObject.destroy();
		}
		m_strands.clear();
		m_vertexScratch.clear();
		m_indexScratch.clear();
		m_indexCount = 0;
	}

} // namespace Demo
