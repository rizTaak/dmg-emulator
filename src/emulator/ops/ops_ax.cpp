#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_ax() {
        add_op(0xa0, +[](Cpu &cpu) -> clock_t {
            //AND B
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0xa1, +[](Cpu &cpu) -> clock_t {
            //AND C
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0xa2, +[](Cpu &cpu) -> clock_t {
            //AND D
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0xa3, +[](Cpu &cpu) -> clock_t {
            //AND E
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0xa4, +[](Cpu &cpu) -> clock_t {
            //AND H
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0xa5, +[](Cpu &cpu) -> clock_t {
            //AND L
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0xa6, +[](Cpu &cpu) -> clock_t {
            //AND (HL)
            //1  8
            //Z 0 1 0
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = and_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xa7, +[](Cpu &cpu) -> clock_t {
            //AND A
            //1  4
            //Z 0 1 0
            cpu.m_a = and_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });

        add_op(0xa8, +[](Cpu &cpu) -> clock_t {
            //XOR B
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0xa9, +[](Cpu &cpu) -> clock_t {
            //XOR C
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0xaa, +[](Cpu &cpu) -> clock_t {
            //XOR D
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0xab, +[](Cpu &cpu) -> clock_t {
            //XOR E
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0xac, +[](Cpu &cpu) -> clock_t {
            //XOR H
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0xad, +[](Cpu &cpu) -> clock_t {
            //XOR L
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0xae, +[](Cpu &cpu) -> clock_t {
            //XOR (HL)
            //1  8
            //Z 0 0 0
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = xor_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xaf, +[](Cpu &cpu) -> clock_t {
            //XOR A
            //1  4
            //Z 0 0 0
            cpu.m_a = xor_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
    }
}