#pragma once

namespace Pekan
{
namespace Graphics
{

	class Shader;

	// A static class for post-processing a frame after rendering before showing it on screen.
	class PostProcessor
	{
	public:

		// Initializes the post processor
		// with a given post-processing shader that will be used to post-process each frame after rendering.
		//
		// NOTE: Given shader MUST have a sampler2D uniform called "screenTexture".
		//       Inside of it the shader will receive the rendered frame.
		static bool init(const char* postProcessingShaderFilepath);

		// A function to be called before rendering a frame
		// that needs to be post-processed.
		static void beginFrame();
		// A function to be called after rendering a frame
		// that needs to be post-processed.
		static void endFrame();

		// Returns (a pointer to) underlying shader.
		// Can be used to set uniforms.
		static Shader* getShader();
	};

} // namespace Graphics
} // namespace Pekan