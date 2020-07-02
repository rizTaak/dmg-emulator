#ifndef EMULATOR_TIME_H
#define EMULATOR_TIME_H

#include <chrono>
#include <emulator/common.h>

namespace emulator {
    constexpr size_t cycle_nanos = 238; //238.418579102
    class RealTime {
        std::chrono::time_point<std::chrono::high_resolution_clock> m_last;
    public:
        void start();
        void sync(clock_t ticks);
    };
}

#endif