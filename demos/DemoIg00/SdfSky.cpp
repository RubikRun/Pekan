#include "SdfSky.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"

#include <cmath>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;

namespace Demo
{

	bool SdfSky::create()
	{
		m_blobs.resize(BLOB_COUNT);

		auto randomX = []() -> float
		{
			constexpr float lo = -35.0f;
			constexpr float hi = 95.0f;
			// Mostly uniform so left and right both get clouds; slight pull toward the far end
			const float t = getRandomFloat(0.0f, 1.0f);
			const float mixed = 0.65f * t + 0.35f * (1.0f - (1.0f - t) * (1.0f - t));
			return lo + (hi - lo) * mixed;
		};

		// Big blue discs — low color variety, scattered above and below grass
		for (int i = 0; i < BLUE_COUNT; i++)
		{
			Blob& b = m_blobs[i];
			b.center = {
				randomX(),
				getRandomFloat(-9.0f, 6.5f)
			};
			const float r = getRandomFloat(2.8f, 5.8f);
			b.radiusX = r * getRandomFloat(0.92f, 1.08f);
			b.radiusY = r * getRandomFloat(0.92f, 1.08f);
			b.edge = getRandomFloat(0.06f, 0.12f);
			const float tone = getRandomFloat(0.0f, 1.0f);
			b.color = {
				0.28f + 0.06f * tone,
				0.55f + 0.08f * tone,
				0.88f + 0.08f * tone,
				0.92f
			};
		}

		// Misty clouds — soft, translucent, denser farther from the start
		for (int i = 0; i < CLOUD_COUNT; i++)
		{
			Blob& b = m_blobs[BLUE_COUNT + i];
			b.center = {
				randomX(),
				getRandomFloat(-8.5f, 6.0f)
			};
			b.radiusX = getRandomFloat(1.6f, 4.0f);
			b.radiusY = getRandomFloat(0.65f, 1.7f);
			b.edge = getRandomFloat(0.14f, 0.28f);
			const float mist = getRandomFloat(0.0f, 1.0f);
			const float brightness = 0.82f + 0.10f * mist;
			b.color = {
				brightness,
				brightness,
				brightness + 0.02f,
				0.22f + 0.28f * mist
			};
		}

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/sdf_sky_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/sdf_sky_fs.glsl");
		if (vs.empty() || fs.empty())
		{
			PK_LOG_ERROR("Failed to load SDF sky shaders.", "DemoIg00");
			return false;
		}

		m_renderObject.create(
			{
				{ ShaderDataType::Float2, "aPosition" },
				{ ShaderDataType::Float2, "aLocal" },
				{ ShaderDataType::Float4, "aColor" },
				{ ShaderDataType::Float3, "aParams" }
			},
			vs.c_str(),
			fs.c_str()
		);

		// pos2 + local2 + color4 + params3 = 11 floats/vert
		m_vertexScratch.resize(static_cast<size_t>(BLOB_COUNT) * 4 * 11);
		m_indexScratch.resize(static_cast<size_t>(BLOB_COUNT) * 6);
		for (int i = 0; i < BLOB_COUNT; i++)
		{
			const unsigned base = static_cast<unsigned>(i * 4);
			const int idx = i * 6;
			m_indexScratch[idx + 0] = base + 0;
			m_indexScratch[idx + 1] = base + 1;
			m_indexScratch[idx + 2] = base + 2;
			m_indexScratch[idx + 3] = base + 0;
			m_indexScratch[idx + 4] = base + 2;
			m_indexScratch[idx + 5] = base + 3;
		}

		m_renderObject.setIndexData(
			m_indexScratch.data(),
			static_cast<long long>(m_indexScratch.size() * sizeof(unsigned)),
			BufferDataUsage::StaticDraw
		);

		uploadVertices();
		return true;
	}

	void SdfSky::uploadVertices() const
	{
		float* v = m_vertexScratch.data();
		constexpr float pad = 1.35f; // room for funky SDF border

		auto emit = [&](glm::vec2 world, glm::vec2 local, const glm::vec4& color, glm::vec3 params)
		{
			*v++ = world.x;
			*v++ = world.y;
			*v++ = local.x;
			*v++ = local.y;
			*v++ = color.r;
			*v++ = color.g;
			*v++ = color.b;
			*v++ = color.a;
			*v++ = params.x;
			*v++ = params.y;
			*v++ = params.z;
		};

		for (const Blob& b : m_blobs)
		{
			const float hx = b.radiusX * pad;
			const float hy = b.radiusY * pad;
			const glm::vec3 params = { b.radiusX, b.radiusY, b.edge };
			const glm::vec2 corners[4] = {
				{ -hx, -hy },
				{  hx, -hy },
				{  hx,  hy },
				{ -hx,  hy }
			};
			for (const glm::vec2& local : corners)
			{
				emit(b.center + local, local, b.color, params);
			}
		}

		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float)),
			BufferDataUsage::StaticDraw
		);
	}

	void SdfSky::render() const
	{
		if (!m_renderObject.isValid())
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

	void SdfSky::destroy()
	{
		if (m_renderObject.isValid())
		{
			m_renderObject.destroy();
		}
		m_blobs.clear();
		m_vertexScratch.clear();
		m_indexScratch.clear();
	}

} // namespace Demo
