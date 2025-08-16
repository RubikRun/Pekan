#pragma once

#include <chrono>
#include <thread>

// We have 2 different implementations of the FPS limiter:
//
// 1. Sleep Compensate
// Each frame this implementation tries to sleep the exact amount needed
// to reach the target frame duration (1 / target FPS).
// Making the CPU sleep is not very accurate because of OS scheduling.
// It often sleeps for a bit more than requested, sometimes for a bit less.
// What we do in this implementation is we keep track of how much it overslept or underslept
// and use that accumulated amount to adjust the sleep next time,
// such that the average FPS is as close as possible to the target FPS.
//
// 2. Wait Blocking
// Each frame this implementation actively waits in a loop, without sleeping,
// the exact amount needed so that the current frame takes 1 / target FPS.
//
// Which one to use?
// - Sleep Compensate has lower CPU usage (like 10x lower),
//   but it doesn't hit the target FPS perfectly - resulting FPS might be different from target FPS by 1-2 frames.
//   There is also sometimes a little "stutter" where 2 frames are rendered immediately one after the other,
//   but it's very rarely noticable.
// - Wait Blocking can be used for a perfectly smooth motion and hitting the target FPS perfectly at the cost of higher CPU usage (like 10x higher)
//
// Toggle these macros on/off to choose between the 2 implementations of FpsLimiter - Sleep Compensate and Sleep Loop
#define PEKAN_FPS_LIMITER_IMPL_SLEEP_COMPENSATE 0
#define PEKAN_FPS_LIMITER_IMPL_WAIT_BLOCKING 1

namespace Pekan
{

#if PEKAN_FPS_LIMITER_IMPL_SLEEP_COMPENSATE
    // A class used to limit FPS in a running application
    // by waiting some amount of time between frames.
    class FpsLimiter
    {
    public:
        FpsLimiter(double targetFps);

        // Waits some amount of time until enough time has passed
        // since the last call to wait() such that we hit the target FPS
        void wait();

    private:
        double m_targetFrameDuration;
        std::chrono::high_resolution_clock::time_point m_lastTimePoint;
        double m_oversleptSum;
    };
#endif

#if PEKAN_FPS_LIMITER_IMPL_WAIT_BLOCKING

    // A class used to limit FPS in a running application
    // by waiting some amount of time between frames.
    class FpsLimiter
    {
    public:
        FpsLimiter(double targetFps);

        // Waits some amount of time until enough time has passed
        // since the last call to wait() such that we hit the target FPS
        void wait();

    private:
        double m_targetFrameDuration;
        std::chrono::high_resolution_clock::time_point m_lastTimePoint;
    };
#endif

} // namespace Pekan