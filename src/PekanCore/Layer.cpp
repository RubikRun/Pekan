#include "Layer.h"

#include "Logger/PekanLogger.h"
#include "PekanApplication.h"

namespace Pekan
{

	void Layer::stopRunningApplication()
	{
		if (m_application)
		{
			m_application->stopRunning();
		}
		else
		{
			PK_ASSERT(false, "A layer is trying to stop running the application, but the layer's application pointer is null.", "Pekan");
		}
	}

} // namespace Pekan