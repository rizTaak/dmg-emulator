
#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_bx() {
        add_op(0xb0, +[](Cpu &cpu) -> clock_t {
            //OR B
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0xb1, +[](Cpu &cpu) -> clock_t {
            //OR C
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0xb2, +[](Cpu &cpu) -> clock_t {
            //OR D
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0xb3, +[](Cpu &cpu) -> clock_t {
            //OR E
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0xb4, +[](Cpu &cpu) -> clock_t {
            //OR H
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0xb5, +[](Cpu &cpu) -> clock_t {
            //OR L
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0xb6, +[](Cpu &cpu) -> clock_t {
            //OR (HL)
            //1  8
            //Z 0 0 0
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = or_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xb7, +[](Cpu &cpu) -> clock_t {
            //OR A
            //1  4
            //Z 0 0 0
            cpu.m_a = or_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
        add_op(0xb8, +[](Cpu &cpu) -> clock_t {
            //CP B
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0xb9, +[](Cpu &cpu) -> clock_t {
            //CP C
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0xba, +[](Cpu &cpu) -> clock_t {
            //CP D
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0xbb, +[](Cpu &cpu) -> clock_t {
            //CP E
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0xbc, +[](Cpu &cpu) -> clock_t {
            //CP H
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0xbd, +[](Cpu &cpu) -> clock_t {
            //CP L
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0xbe, +[](Cpu &cpu) -> clock_t {
            //CP (HL)
            //1  8
            //Z 1 H C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            sub_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xbf, +[](Cpu &cpu) -> clock_t {
            //CP A
            //1  4
            //Z 1 H C
            sub_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
    }
}