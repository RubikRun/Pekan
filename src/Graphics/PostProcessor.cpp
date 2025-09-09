#include "PostProcessor.h"

#include "GLCall.h"
#include "RenderObject.h"
#include "FrameBuffer.h"
#include "Utils/FileUtils.h"
#include "PekanLogger.h"
#include "PekanEngine.h"
#include "PekanApplication.h"

#define VERTEX_SHADER_FILEPATH PEKAN_GRAPHICS_ROOT_DIR "/Shaders/PostProcessor_VertexShader.glsl"

namespace Pekan
{
namespace Graphics
{

	// A flag indicating if the post processor has been initialized
	static bool g_isInitialized = false;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// The flow of data here is basically this:
	//     -> draw call
	//     -> g_frameBufferMultisample    (only if using multisample rendering)
	//     -> g_frameBufferFinal
	//     -> g_renderObject with a post-processing shader applied
	//     -> screen
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// An intermediate frame buffer that will be used to render multisample frames.
	// A multisample frame has to then be transferred to the final frame buffer before applying the post-processing shader.
	static FrameBuffer g_frameBufferMultisample;

	// The final frame buffer that will be passed to the post-processing shader.
	static FrameBuffer g_frameBufferFinal;

	// Vertices of a rectangle covering the whole window/viewport
	static constexpr float RECTANGLE_VERTICES[] =
	{
		// position     // texture coordinate
		-1.0f, -1.0f,    0.0f, 0.0f,
		1.0f, -1.0f,     1.0f, 0.0f,
		1.0f, 1.0f,      1.0f, 1.0f,
		-1.0f, 1.0f,     0.0f, 1.0f
	};
	// Indices of a rectangle covering the whole window/viewport
	static constexpr unsigned RECTANGLE_INDICES[] = { 0, 1, 2, 0, 2, 3 };

	// Underlying render object
	// used to render rectangle with post-processed frame.
	static RenderObject g_renderObject;

	// Number of samples per pixel
	static int g_samplesPerPixel = -1;

	bool PostProcessor::init(const char* postProcessingShaderFilepath)
	{
		PK_ASSERT(!g_isInitialized, "Trying to initialize the PostProcessor but it's already initialized.", "Pekan");

		// Get number of samples per pixel from application
		{
			const PekanApplication* application = PekanEngine::getApplication();
			if (application != nullptr)
			{
				g_samplesPerPixel = application->getProperties().numberOfSamples;
			}
			else
			{
				PK_LOG_ERROR("Trying to initialize the PostProcessor but there is no application registered in Pekan.", "Pekan");
				g_samplesPerPixel = 1;
			}
		}

		// Create underlying frame buffers with the size of the window
		const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();
		if (g_samplesPerPixel > 1)
		{
			g_frameBufferMultisample.create(windowSize.x, windowSize.y, g_samplesPerPixel);
		}
		g_frameBufferFinal.create(windowSize.x, windowSize.y, 1);

		// Create underlying render object with rectangle's vertices,
		// default vertex shader, and given fragment shader
		g_renderObject.create
		(
			RECTANGLE_VERTICES, 4 * 2 * 2 * sizeof(float),
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "textureCoordinates"} },
			BufferDataUsage::StaticDraw,
			FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readTextFileToString(postProcessingShaderFilepath).c_str()
		);
		g_renderObject.setIndexData(RECTANGLE_INDICES, 6 * sizeof(unsigned));
		// Set "screenTexture" uniform inside the shader to 0,
		// because we will always bind the frame buffer's texture on slot 0
		g_renderObject.getShader().setUniform1i("screenTexture", 0);

		g_isInitialized = true;
		return true;
	}

	void PostProcessor::beginFrame()
	{
		PK_ASSERT(g_isInitialized, "Trying to begin frame with the PostProcessor but it's not yet initialized.", "Pekan");

		// Bind the correct frame buffer depending on samples per pixel
		if (g_samplesPerPixel > 1)
		{
			g_frameBufferMultisample.bind();
		}
		else
		{
			g_frameBufferFinal.bind();
		}
		// Clear both color and depth from frame buffer
		RenderCommands::clear(true, true);
	}

	void PostProcessor::endFrame()
	{
		PK_ASSERT(g_isInitialized, "Trying to end frame with the PostProcessor but it's not yet initialized.", "Pekan");

		// If we are using the multisample frame buffer,
		// resolve it to the final frame buffer.
		// This will transform all pixel data from multisample to single-sample
		// and copy it to the final frame buffer
		if (g_samplesPerPixel > 1)
		{
			g_frameBufferMultisample.resolveMultisampleToSinglesample(g_frameBufferFinal);
		}

		// Unbind our frame buffer,
		// effectively binding the default frame buffer which is the screen.
		// (It doesn't matter which one we unbind)
		g_frameBufferFinal.unbind();

		// If depth testing is enabled, disable it as we don't need it to render the post-processed result onto the rectangle
		bool originalIsEnabledDepthTest = RenderState::isEnabledDepthTest();
		if (originalIsEnabledDepthTest)
		{
			RenderState::disableDepthTest();
		}

		// Bind final frame buffer's texture containing the rendered frame,
		// because we want to access it in the post-processing shader.
		// (Importantly, bind it to slot 0 because shader expects it there)
		g_frameBufferFinal.bindTexture(0);
		// Render the rectangle using the post-processing shader and the texture containing the rendered frame
		g_renderObject.render();

		// If depth testing was originally enabled, enable it again
		if (originalIsEnabledDepthTest)
		{
			RenderState::enableDepthTest();
		}
	}

	Shader* PostProcessor::getShader()
	{
		return &g_renderObject.getShader();
	}

} // namespace Graphics
} // namespace Pekan