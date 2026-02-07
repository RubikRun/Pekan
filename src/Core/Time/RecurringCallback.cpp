#include "RecurringCallback.h"

namespace Pekan
{

	RecurringCallback::RecurringCallback
	(
		std::function<void()> callback,
		float interval
	)
		: callback(std::move(callback))
		, interval(interval)
	{
	}

} // namespace Pekan