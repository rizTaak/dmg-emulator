
#include <iostream>
#include <thread>
#include <emulator/time.h>

namespace emulator {
    void RealTime::start() {
        m_last = std::chrono::high_resolution_clock::now();
    }

    void RealTime::sync(clock_t ticks) {
        auto cpu_time = std::chrono::nanoseconds(ticks * cycle_nanos);
        auto now = std::chrono::high_resolution_clock::now();
        auto actual_time = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_last);
        while (actual_time < cpu_time) {
            now = std::chrono::high_resolution_clock::now();
            actual_time = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_last);
        }
        m_last = m_last + cpu_time;
    }
}