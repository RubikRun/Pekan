#include "Time/FpsLimiter.h"

using namespace std::chrono;
using TimePoint = std::chrono::high_resolution_clock::time_point;
using Duration = std::chrono::duration<double>;

namespace Pekan
{

#if PEKAN_FPS_LIMITER_IMPL_SLEEP_COMPENSATE

    FpsLimiter::FpsLimiter(double targetFps)
        : m_targetFrameDuration(1.0 / targetFps)
        , m_lastTimePoint(high_resolution_clock::now())
        , m_oversleptSum(0.0)
    {}

    void FpsLimiter::wait()
    {
        const Duration elapsed = high_resolution_clock::now() - m_lastTimePoint;

        const double targetSleepTime = m_targetFrameDuration - elapsed.count() - m_oversleptSum;

        if (targetSleepTime > 0.0)
        {
            const TimePoint sleepStart = high_resolution_clock::now();
            std::this_thread::sleep_for(Duration(targetSleepTime));
            const TimePoint sleepEnd = high_resolution_clock::now();
            const double actualSleepTime = Duration(sleepEnd - sleepStart).count();

            // Measure the actual sleep error
            m_oversleptSum = actualSleepTime - targetSleepTime;
        }
        else
        {
            m_oversleptSum = 0.0;
        }

        m_lastTimePoint = high_resolution_clock::now();
    }

#endif

#if PEKAN_FPS_LIMITER_IMPL_WAIT_BLOCKING

    FpsLimiter::FpsLimiter(double targetFps)
        : m_targetFrameDuration(1.0 / targetFps)
        , m_lastTimePoint(high_resolution_clock::now())
    {
    }

    void FpsLimiter::wait()
    {
        while (Duration(high_resolution_clock::now() - m_lastTimePoint).count() < m_targetFrameDuration)
        { /* nothing */ }

        m_lastTimePoint = high_resolution_clock::now();
    }

#endif

} // namespace Pekan