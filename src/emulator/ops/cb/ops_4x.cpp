#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb4x() {
        add_bc_op(0x40, +[](Cpu &cpu) -> clock_t {
            //BIT 0,B
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x41, +[](Cpu &cpu) -> clock_t {
            //BIT 0,C
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x42, +[](Cpu &cpu) -> clock_t {
            //BIT 0,D
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x43, +[](Cpu &cpu) -> clock_t {
            //BIT 0,E
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x44, +[](Cpu &cpu) -> clock_t {
            //BIT 0,H
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x45, +[](Cpu &cpu) -> clock_t {
            //BIT 0,L
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x46, +[](Cpu &cpu) -> clock_t {
            //BIT 0,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_0(cpu, imm);
            return 12;
        });
        add_bc_op(0x47, +[](Cpu &cpu) -> clock_t {
            //BIT 0,A
            //2  8
            //Z 0 1 -
            bit_0(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x48, +[](Cpu &cpu) -> clock_t {
            //BIT 1,B
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x49, +[](Cpu &cpu) -> clock_t {
            //BIT 1,C
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x4a, +[](Cpu &cpu) -> clock_t {
            //BIT 1,D
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x4b, +[](Cpu &cpu) -> clock_t {
            //BIT 1,E
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x4c, +[](Cpu &cpu) -> clock_t {
            //BIT 1,H
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x4d, +[](Cpu &cpu) -> clock_t {
            //BIT 1,L
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x4e, +[](Cpu &cpu) -> clock_t {
            //BIT 1,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_1(cpu, imm);
            return 12;
        });
        add_bc_op(0x4f, +[](Cpu &cpu) -> clock_t {
            //BIT 1,A
            //2  8
            //Z 0 1 -
            bit_1(cpu, cpu.m_a);
            return 8;
        });
    }
}