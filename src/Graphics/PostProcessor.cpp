#include "PostProcessor.h"

#include "GLCall.h"
#include "RenderObject.h"
#include "FrameBuffer.h"
#include "Utils/FileUtils.h"
#include "PekanLogger.h"

#define VERTEX_SHADER_FILEPATH PEKAN_GRAPHICS_ROOT_DIR "/Shaders/PostProcessor_VertexShader.glsl"

namespace Pekan
{
namespace Graphics
{

	// A flag indicating if the post processor has been initialized
	static bool g_isInitialized = false;

	// Frame buffer where each frame will be rendered before post-processing
	static FrameBuffer g_frameBuffer;

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

	bool PostProcessor::init(const char* postProcessingShaderFilepath)
	{
		PK_ASSERT(!g_isInitialized, "Trying to initialize the PostProcessor but it's already initialized.", "Pekan");

		// Create underlying frame buffer with the size of the window
		const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();
		g_frameBuffer.create(windowSize.x, windowSize.y);

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

		g_frameBuffer.bind();
		// Clear both color and depth from frame buffer
		GLCall(RenderCommands::clear(true, true));
		// Enable depth testing
		GLCall(RenderState::enableDepthTest());
	}

	void PostProcessor::endFrame()
	{
		PK_ASSERT(g_isInitialized, "Trying to end frame with the PostProcessor but it's not yet initialized.", "Pekan");
		// Unbind our frame buffer containing the rendered frame,
		// effectively binding the default frame buffer which is the screen.
		g_frameBuffer.unbind();

		// Disable depth testing as we don't need it to render the post-processing result on the rectangle.
		GLCall(RenderState::disableDepthTest());

		// Bind frame buffer's texture containing the rendered frame,
		// because we want to access it in the post-processing shader
		g_frameBuffer.bindTexture();
		// Render the rectangle using the post-processing shader and the texture containing the rendered frame
		g_renderObject.render();
	}

} // namespace Graphics
} // namespace Pekan