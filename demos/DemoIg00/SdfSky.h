#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>
#include <vector>

namespace Demo
{

	// Sky made of large blue SDF circles with white SDF clouds on top.
	class SdfSky
	{
	public:
		bool create();
		void render() const;
		void destroy();

	private:
		static constexpr int BLUE_COUNT = 16;
		static constexpr int CLOUD_COUNT = 90;
		static constexpr int BLOB_COUNT = BLUE_COUNT + CLOUD_COUNT;

		struct Blob
		{
			glm::vec2 center = { 0.0f, 0.0f };
			float radiusX = 1.0f;
			float radiusY = 1.0f;
			float edge = 0.08f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		};

		void uploadVertices() const;

		std::vector<Blob> m_blobs;

		mutable Pekan::Graphics::RenderObject m_renderObject;
		mutable std::vector<float> m_vertexScratch;
		mutable std::vector<unsigned> m_indexScratch;
	};

} // namespace Demo
