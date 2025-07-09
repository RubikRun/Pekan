#include "Renderer2D.h"

#include "Utils/FileUtils.h"
#include "SubsystemManager.h"
#include "Graphics.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/2D_BatchRendering_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/SolidColor_BatchRendering_FragmentShader.glsl"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer
{

	static Renderer2D g_renderer2D;
	
	void Renderer2D::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2D);
	}

	Renderer2D::CameraWeakPtr Renderer2D::s_camera;
	RenderObject Renderer2D::s_batchDynamic;
	std::vector<ShapeVertex> Renderer2D::s_verticesDynamic;
	std::vector<unsigned> Renderer2D::s_indicesDynamic;
	RenderObject Renderer2D::s_batchStatic;
	std::vector<ShapeVertex> Renderer2D::s_verticesStatic;
	std::vector<unsigned> Renderer2D::s_indicesStatic;

	void Renderer2D::beginFrame()
	{
		s_verticesDynamic.clear();
		s_indicesDynamic.clear();
		s_verticesStatic.clear();
		s_indicesStatic.clear();
	}

	void Renderer2D::endFrame()
	{
		/////////////////////
		// DRAW DYNAMIC BATCH
		/////////////////////
		{
			s_batchDynamic.setVertexData(s_verticesDynamic.data(), s_verticesDynamic.size() * sizeof(ShapeVertex));
			s_batchDynamic.setIndexData(s_indicesDynamic.data(), s_indicesDynamic.size() * sizeof(unsigned));

			CameraSharedPtr camera = s_camera.lock();
			// If camera still exists use its transform to set shader's view projection matrix uniform
			if (camera != nullptr)
			{
				const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
				s_batchDynamic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);
			}
			// Otherwise, use a default view projection matrix
			else
			{
				static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
				s_batchDynamic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);
			}

			RenderCommands::drawIndexed(s_indicesDynamic.size());
		}

		/////////////////////
		// DRAW STATIC BATCH
		/////////////////////
		{
			s_batchStatic.setVertexData(s_verticesStatic.data(), s_verticesStatic.size() * sizeof(ShapeVertex));
			s_batchStatic.setIndexData(s_indicesStatic.data(), s_indicesStatic.size() * sizeof(unsigned));

			CameraSharedPtr camera = s_camera.lock();
			// If camera still exists use its transform to set shader's view projection matrix uniform
			if (camera != nullptr)
			{
				const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
				s_batchStatic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);
			}
			// Otherwise, use a default view projection matrix
			else
			{
				static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
				s_batchStatic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);
			}

			RenderCommands::drawIndexed(s_indicesStatic.size());
		}
	}

	void Renderer2D::init()
	{
		///////////////////////////
		// INITIALIZE DYNAMIC BATCH
		///////////////////////////
		{
			// Create shapes batch with empty vertex data
			s_batchDynamic.create
			(
				s_verticesDynamic.data(),
				0,
				{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
				BufferDataUsage::DynamicDraw,
				FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
				FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
			);
			// Set empty index data to shapes batch
			s_batchDynamic.setIndexData(s_indicesDynamic.data(), 0, BufferDataUsage::DynamicDraw);

			// Set shader's view projection matrix uniform to a default view projection matrix
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			s_batchDynamic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);
		}

		///////////////////////////
		// INITIALIZE STATIC BATCH
		///////////////////////////
		{
			// Create shapes batch with empty vertex data
			s_batchStatic.create
			(
				s_verticesStatic.data(),
				0,
				{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
				BufferDataUsage::StaticDraw,
				FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
				FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
			);
			// Set empty index data to shapes batch
			s_batchStatic.setIndexData(s_indicesStatic.data(), 0, BufferDataUsage::StaticDraw);

			// Set shader's view projection matrix uniform to a default view projection matrix
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			s_batchStatic.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);
		}

		m_isInitialized = true;
	}

	void Renderer2D::exit()
	{
		s_batchDynamic.destroy();
		s_verticesDynamic.clear();
		s_indicesDynamic.clear();
		s_batchStatic.destroy();
		s_verticesStatic.clear();
		s_indicesStatic.clear();

		m_isInitialized = false;
	}

	ISubsystem* Renderer2D::getParent()
	{
		return Graphics::Graphics::getInstance();
	}

	void Renderer2D::render(const Shape& shape)
	{
		if (shape.isDynamic())
		{
			const unsigned oldVerticesSize = unsigned(s_verticesDynamic.size());
			const size_t oldIndicesSize = s_indicesDynamic.size();

			// Get shape's vertices
			const ShapeVertex* vertices = shape.getVertices();
			const int verticesCount = shape.getVerticesCount();
			// Add shape's vertices to the batch
			s_verticesDynamic.insert(s_verticesDynamic.end(), vertices, vertices + verticesCount);

			// Get shape's indices
			const unsigned* localIndices = shape.getIndices();
			const int indicesCount = shape.getIndicesCount();
			// Add shape's indices to the batch,
			// adding the old size of vertices list (before adding the new vertices) to each index,
			// so that they correctly reference vertices from the batch.
			s_indicesDynamic.resize(s_indicesDynamic.size() + indicesCount);
			for (size_t i = oldIndicesSize; i < s_indicesDynamic.size(); i++)
			{
				s_indicesDynamic[i] = localIndices[i - oldIndicesSize] + oldVerticesSize;
			}
		}
		else
		{
			const unsigned oldVerticesSize = unsigned(s_verticesStatic.size());
			const size_t oldIndicesSize = s_indicesStatic.size();

			// Get shape's vertices
			const ShapeVertex* vertices = shape.getVertices();
			const int verticesCount = shape.getVerticesCount();
			// Add shape's vertices to the batch
			s_verticesStatic.insert(s_verticesStatic.end(), vertices, vertices + verticesCount);

			// Get shape's indices
			const unsigned* localIndices = shape.getIndices();
			const int indicesCount = shape.getIndicesCount();
			// Add shape's indices to the batch,
			// adding the old size of vertices list (before adding the new vertices) to each index,
			// so that they correctly reference vertices from the batch.
			s_indicesStatic.resize(s_indicesStatic.size() + indicesCount);
			for (size_t i = oldIndicesSize; i < s_indicesStatic.size(); i++)
			{
				s_indicesStatic[i] = localIndices[i - oldIndicesSize] + oldVerticesSize;
			}
		}
	}

} // namespace Renderer
} // namespace Pekan