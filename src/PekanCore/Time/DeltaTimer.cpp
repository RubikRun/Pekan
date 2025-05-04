#include "DeltaTimer.h"

using std::chrono::high_resolution_clock;
using TimePoint = std::chrono::high_resolution_clock::time_point;
using Duration = std::chrono::duration<double>;

namespace Pekan
{

	double DeltaTimer::getDeltaTime()
	{
		const TimePoint now = high_resolution_clock::now();
		if (!m_used)
		{
			m_lastTimePoint = now;
			m_used = true;
			return 0.0;
		}

		// Delta time is the difference between now and last time
		const double deltaTime = Duration(now - m_lastTimePoint).count();
		m_lastTimePoint = now;

		return deltaTime;
	}

} // namespace Pekan