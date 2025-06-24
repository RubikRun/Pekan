#pragma once

#include "RenderState.h"
#include "RenderComponent.h"

namespace Pekan
{
namespace Renderer
{

	// A class representing an index buffer in Pekan's renderer.
	// An index buffer holds a buffer of data that can be used as indices into a vertex buffer.
	class IndexBuffer : public RenderComponent
	{
	public:

		~IndexBuffer();

		// Make base class RenderComponent's version of create() be visible in this derived class
		using RenderComponent::create;
		// Creates the underlying index buffer object, fills it with given data, and binds it
		void create(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);

		// Fills index buffer with given data. Any previous data is removed.
		void setData(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);

		// Fills a region of the index buffer with given data. Previous data in this region is overwritten.
		// @param[in] data - Data to be filled in to the region
		// @param[in] offset - Offset from the beginning of the index buffer to where the region begins
		// @param[in] size - Size of the region. Should match the size of given data.
		void setSubData(const void* data, long long offset, long long size);

		void bind() const override;
		void unbind() const override;

	private: /* functions */

		void _create() override;
		void _destroy() override;
	};

} // namespace Renderer
} // namespace Pekan