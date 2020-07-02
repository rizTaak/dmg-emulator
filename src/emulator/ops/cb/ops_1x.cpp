#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb1x() {
        add_bc_op(0x10, +[](Cpu &cpu) -> clock_t {
            //RL B
            //2  8
            //Z 0 0 C
            cpu.m_b = rl_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x11, +[](Cpu &cpu) -> clock_t {
            //RL C
            //2  8
            //Z 0 0 C
            cpu.m_c = rl_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x12, +[](Cpu &cpu) -> clock_t {
            //RL D
            //2  8
            //Z 0 0 C
            cpu.m_d = rl_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x13, +[](Cpu &cpu) -> clock_t {
            //RL E
            //2  8
            //Z 0 0 C
            cpu.m_e = rl_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x14, +[](Cpu &cpu) -> clock_t {
            //RL H
            //2  8
            //Z 0 0 C
            cpu.m_h = rl_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x15, +[](Cpu &cpu) -> clock_t {
            //RL L
            //2  8
            //Z 0 0 C
            cpu.m_l = rl_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x16, +[](Cpu &cpu) -> clock_t {
            //RL (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, rl_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x17, +[](Cpu &cpu) -> clock_t {
            //RL A
            //2  8
            //Z 0 0 C
            cpu.m_a = rl_n(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x18, +[](Cpu &cpu) -> clock_t {
            //RR B
            //2  8
            //Z 0 0 C
            cpu.m_b = rr_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x19, +[](Cpu &cpu) -> clock_t {
            //RR C
            //2  8
            //Z 0 0 C
            cpu.m_c = rr_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x1a, +[](Cpu &cpu) -> clock_t {
            //RR D
            //2  8
            //Z 0 0 C
            cpu.m_d = rr_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x1b, +[](Cpu &cpu) -> clock_t {
            //RR E
            //2  8
            //Z 0 0 C
            cpu.m_e = rr_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x1c, +[](Cpu &cpu) -> clock_t {
            //RR H
            //2  8
            //Z 0 0 C
            cpu.m_h = rr_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x1d, +[](Cpu &cpu) -> clock_t {
            //RR L
            //2  8
            //Z 0 0 C
            cpu.m_l = rr_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x1e, +[](Cpu &cpu) -> clock_t {
            //RR (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, rr_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x1f, +[](Cpu &cpu) -> clock_t {
            //RR A
            //2  8
            //Z 0 0 C
            cpu.m_a = rr_n(cpu, cpu.m_a);
            return 8;
        });
    }
}