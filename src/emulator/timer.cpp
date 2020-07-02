
#include <emulator/memory.h>
#include <emulator/cpu.h>
#include <emulator/timer.h>

namespace emulator {
    void Timer::step(clock_t ticks) {
        update_div_counter(ticks);
        if (clock_enabled()) {
            m_counter += ticks;
            uint32_t threshold = 0;
            auto freq = get_frequency();
            switch (freq)
            {
                case 0: threshold = 1024 ; break ; // freq 4096
                case 1: threshold = 16 ; break ;// freq 262144
                case 2: threshold = 64 ; break ;// freq 65536
                case 3: threshold = 256 ; break ;// freq 16382
                default:
                    throw std::runtime_error("wtf timer!!!");
            }
            while (m_counter >= threshold) {
                m_counter -= threshold;
                m_tima_counter += 1;
                if (m_tima_counter == 0x00) {
                    m_tima_counter = m_tma_modulo;
                    m_cpu.timer_set_interrupt();
                }
            }
        }
    }

    void Timer::update_div_counter(clock_t ticks) {
        m_div_counter += ticks;
        if (m_div_counter > 255) {
            m_div_counter -= 255;
            m_timer_divider++;
        }
    }
};