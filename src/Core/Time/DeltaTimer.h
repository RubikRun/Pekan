#pragma once

#include <chrono>
#include <thread>

namespace Pekan
{

    class DeltaTimer
    {
    public:

        // Returns "delta time" in milliseconds,
        // meaning the time that has passed since this function was last called.
        //
        // NOTE: First time when you call this function it will return 0.0
        double getDeltaTime();

    private:
        std::chrono::high_resolution_clock::time_point m_lastTimePoint;

        // Flag indicating if the delta timer has been used yet.
        bool m_used = false;
    };

} // namespace Pekan