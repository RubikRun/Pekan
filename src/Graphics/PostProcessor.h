#pragma once

namespace Pekan
{
namespace Graphics
{

	class Shader;

	// A static class for post-processing a frame after rendering before showing it on screen.
	class PostProcessor
	{
		// Make GraphicsSubsystem a friend so that it can exit PostProcessor when GraphicsSubsystem is exited.
		friend class GraphicsSubsystem;

	public:

		// Sets a given post-processing shader to be used for post-processing each frame after rendering.
		// If needed, initializes the PostProcessor first.
		//
		// NOTE: Given shader MUST have a sampler2D uniform called "screenTexture".
		//       Inside of it the shader will receive the rendered frame.
		static void setPostProcessingShader(const char* postProcessingShaderFilepath);

		// A function to be called before rendering a frame.
		// Does nothing if no post-processing shader has been set.
		static void beginFrame();
		// A function to be called after rendering a frame.
		// Does nothing if no post-processing shader has been set.
		static void endFrame();

		// Returns (a pointer to) underlying shader.
		// Can be used to set uniforms.
		static Shader* getShader();

	private:

		// Cleans up PostProcessor resources. Must be called before OpenGL context destruction.
		// Only GraphicsSubsystem should call this.
		static void exit();
	};

} // namespace Graphics
} // namespace Pekan