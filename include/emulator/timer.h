#ifndef EMULATOR_TIMER_H
#define EMULATOR_TIMER_H

namespace emulator {
    class Cpu;
    class Memory;

    class Timer {
        Cpu &m_cpu;
        Memory &m_mem;
        uint32_t m_counter{};
        uint32_t m_div_counter{};

        register_8_t &m_timer_divider;
        register_8_t &m_tima_counter;
        register_8_t &m_tma_modulo;
        register_8_t &m_tac_control;
    public:
        Timer(Memory &mem, Cpu &cpu):
            m_mem {mem},
            m_cpu {cpu},
            m_tima_counter {mem.get_ref_byte(mem_registers::tima_timer_counter)},
            m_tma_modulo {mem.get_ref_byte(mem_registers::tma_timer_modulo)},
            m_timer_divider {mem.get_ref_byte(mem_registers::timer_divider)},
            m_tac_control {mem.get_ref_byte(mem_registers::tac_timer_control)}{
        }

        void step(clock_t ticks);

        [[nodiscard]] inline bool clock_enabled() const {
            return (m_tac_control & 0b0000'0100u) > 0x0u;
        }

        [[nodiscard]] inline register_8_t get_frequency() const {
            return (m_tac_control & 0b0000'0011u);
        }

        void update_div_counter(clock_t ticks);
    };
}

#endif