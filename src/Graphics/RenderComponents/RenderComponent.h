#pragma once

#include "RenderState.h"

namespace Pekan
{
namespace Graphics
{

	// A base class representing a render component.
	// A render component is any type of object that exists on the GPU as part of the rendering process.
	// (e.g. shaders, vertex buffers, vertex arrays)
	class RenderComponent
	{
	public:

		virtual ~RenderComponent();

		// Creates the underlying GPU object and binds it, unless the doBind flag is false
		void create(bool doBind = true);
		// Deletes the GPU object and unbinds it
		void destroy();

		// Checks if object is valid, meaning that it has been successfully created and not yet destroyed
		inline bool isValid() const { return m_id != 0; }

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	private: /* functions */

		virtual void _create() = 0;
		virtual void _destroy() = 0;

	protected: /* variables */

		// ID of GPU object
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan