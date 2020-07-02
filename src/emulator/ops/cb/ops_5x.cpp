#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb5x() {
        add_bc_op(0x50, +[](Cpu &cpu) -> clock_t {
            //BIT 2,B
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x51, +[](Cpu &cpu) -> clock_t {
            //BIT 2,C
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x52, +[](Cpu &cpu) -> clock_t {
            //BIT 2,D
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x53, +[](Cpu &cpu) -> clock_t {
            //BIT 2,E
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x54, +[](Cpu &cpu) -> clock_t {
            //BIT 2,H
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x55, +[](Cpu &cpu) -> clock_t {
            //BIT 2,L
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x56, +[](Cpu &cpu) -> clock_t {
            //BIT 2,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_2(cpu, imm);
            return 12;
        });
        add_bc_op(0x57, +[](Cpu &cpu) -> clock_t {
            //BIT 2,A
            //2  8
            //Z 0 1 -
            bit_2(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x58, +[](Cpu &cpu) -> clock_t {
            //BIT 3,B
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x59, +[](Cpu &cpu) -> clock_t {
            //BIT 3,C
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x5a, +[](Cpu &cpu) -> clock_t {
            //BIT 3,D
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x5b, +[](Cpu &cpu) -> clock_t {
            //BIT 3,E
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x5c, +[](Cpu &cpu) -> clock_t {
            //BIT 3,H
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x5d, +[](Cpu &cpu) -> clock_t {
            //BIT 3,L
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x5e, +[](Cpu &cpu) -> clock_t {
            //BIT 3,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_3(cpu, imm);
            return 12;
        });
        add_bc_op(0x5f, +[](Cpu &cpu) -> clock_t {
            //BIT 3,A
            //2  8
            //Z 0 1 -
            bit_3(cpu, cpu.m_a);
            return 8;
        });
    }
}