#include "RenderComponent.h"

#include "Logger/PekanLogger.h"

namespace Pekan
{
	namespace Renderer
	{

		RenderComponent::~RenderComponent()
		{
			PK_ASSERT(!isValid(), "An instance of RenderComponent is getting destroyed but the underlying GPU object is not yet destroyed."
			                      " Make sure to destroy the GPU object by calling destroy() on your RenderComponent instance"
			                      " before that instance itself gets destroyed.", "Pekan");
		}

		void RenderComponent::create(bool doBind)
		{
			if (isValid())
			{
				PK_LOG_WARNING("Creating a render component, but there is already a render component created"
				               " in this RenderComponent instance. Old render component will be deleted.", "Pekan");
				destroy();
			}
			_create();
			if (doBind)
			{
				bind();
			}
		}

		void RenderComponent::destroy()
		{
			if (!isValid())
			{
				PK_ASSERT(false, "Trying to destroy a non-existing render component.", "Pekan");
				return;
			}
			_destroy();
			unbind();
			m_id = 0;
		}

	} // namespace Renderer
} // namespace Pekan