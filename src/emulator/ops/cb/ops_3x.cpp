#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb3x() {
        add_bc_op(0x30, +[](Cpu &cpu) -> clock_t {
            //SWAP B
            //2  8
            //Z 0 0 0
            cpu.m_b = swap_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x31, +[](Cpu &cpu) -> clock_t {
            //SWAP C
            //2  8
            //Z 0 0 0
            cpu.m_c = swap_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x32, +[](Cpu &cpu) -> clock_t {
            //SWAP D
            //2  8
            //Z 0 0 0
            cpu.m_d = swap_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x33, +[](Cpu &cpu) -> clock_t {
            //SWAP E
            //2  8
            //Z 0 0 0
            cpu.m_e = swap_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x34, +[](Cpu &cpu) -> clock_t {
            //SWAP H
            //2  8
            //Z 0 0 0
            cpu.m_h = swap_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x35, +[](Cpu &cpu) -> clock_t {
            //SWAP L
            //2  8
            //Z 0 0 0
            cpu.m_l = swap_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x36, +[](Cpu &cpu) -> clock_t {
            //SWAP (HL)
            //2  16
            //Z 0 0 0
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, swap_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x37, +[](Cpu &cpu) -> clock_t {
            //SWAP A
            //2  8
            //Z 0 0 0
            cpu.m_a = swap_n(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x38, +[](Cpu &cpu) -> clock_t {
            //SRL B
            //2  8
            //Z 0 0 C
            cpu.m_b = srl_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x39, +[](Cpu &cpu) -> clock_t {
            //SRL C
            //2  8
            //Z 0 0 C
            cpu.m_c = srl_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x3a, +[](Cpu &cpu) -> clock_t {
            //SRL D
            //2  8
            //Z 0 0 C
            cpu.m_d = srl_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x3b, +[](Cpu &cpu) -> clock_t {
            //SRL E
            //2  8
            //Z 0 0 C
            cpu.m_e = srl_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x3c, +[](Cpu &cpu) -> clock_t {
            //SRL H
            //2  8
            //Z 0 0 C
            cpu.m_h = srl_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x3d, +[](Cpu &cpu) -> clock_t {
            //SRL L
            //2  8
            //Z 0 0 C
            cpu.m_l = srl_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x3e, +[](Cpu &cpu) -> clock_t {
            //SRL (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, srl_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x3f, +[](Cpu &cpu) -> clock_t {
            //SRL A
            //2  8
            //Z 0 0 C
            cpu.m_a = srl_n(cpu, cpu.m_a);
            return 8;
        });
    }
}