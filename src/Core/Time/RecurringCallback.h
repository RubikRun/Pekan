#pragma once

#include <functional>

namespace Pekan
{

	// A struct representing a callback function
	// that is repeatedly called every N seconds.
	struct RecurringCallback
	{

		// Creates a recurring callback from a given callback function
		// and a given time interval between calls.
		// @param[in] - time interval between calls (in seconds)
		RecurringCallback(std::function<void()> callback, float interval);

		// A function to be called when the time interval elapses
		std::function<void()> callback;
		// Time interval between calls
		float interval = 0.0f;
		// Time elapsed since last call
		float elapsed = 0.0f;
	};

} // namespace Pekan