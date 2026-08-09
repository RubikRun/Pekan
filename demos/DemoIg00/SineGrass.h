#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>
#include <vector>

namespace Demo
{

	// Ground rendered as many vertical sine-wave grass strands.
	class SineGrass
	{
	public:
		// Spawns strands from random points inside the given ground rectangle.
		bool create(float centerX, float centerY, float width, float height);
		void update(float dt);
		void render() const;
		void destroy();

	private:
		static constexpr int STRAND_COUNT = 26000;
		static constexpr int SEGMENTS_PER_STRAND = 22;

		struct Strand
		{
			float baseX = 0.0f;
			float baseY = 0.0f;
			float length = 0.25f;
			float amplitude = 0.04f;   // sine height → horizontal blade width
			float thickness = 0.03f;  // stroke thickness
			float oscillations = 3.0f;
			float angle = 0.0f;       // tilt from vertical, radians
			glm::vec3 color = { 0.25f, 0.55f, 0.25f };
			float phase = 0.0f;
			float scrollSpeed = 0.05f;
		};

		void uploadVertices() const;

		std::vector<Strand> m_strands;

		mutable Pekan::Graphics::RenderObject m_renderObject;
		mutable std::vector<float> m_vertexScratch;
		mutable std::vector<unsigned> m_indexScratch;
		mutable int m_indexCount = 0;
	};

} // namespace Demo
