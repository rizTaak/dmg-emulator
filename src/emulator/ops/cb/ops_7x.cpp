#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb7x() {
        add_bc_op(0x70, +[](Cpu &cpu) -> clock_t {
            //BIT 6,B
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x71, +[](Cpu &cpu) -> clock_t {
            //BIT 6,C
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x72, +[](Cpu &cpu) -> clock_t {
            //BIT 6,D
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x73, +[](Cpu &cpu) -> clock_t {
            //BIT 6,E
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x74, +[](Cpu &cpu) -> clock_t {
            //BIT 6,H
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x75, +[](Cpu &cpu) -> clock_t {
            //BIT 6,L
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x76, +[](Cpu &cpu) -> clock_t {
            //BIT 6,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_6(cpu, imm);
            return 12;
        });
        add_bc_op(0x77, +[](Cpu &cpu) -> clock_t {
            //BIT 6,A
            //2  8
            //Z 0 1 -
            bit_6(cpu, cpu.m_a);
            return 8;
        });

        // BIT 7
        add_bc_op(0x78, +[](Cpu &cpu) -> clock_t {
            //BIT 7,B
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x79, +[](Cpu &cpu) -> clock_t {
            //BIT 7,C
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x7a, +[](Cpu &cpu) -> clock_t {
            //BIT 7,D
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x7b, +[](Cpu &cpu) -> clock_t {
            //BIT 7,E
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x7c, +[](Cpu &cpu) -> clock_t {
            //BIT 7,H
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x7d, +[](Cpu &cpu) -> clock_t {
            //BIT 7,L
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x7e, +[](Cpu &cpu) -> clock_t {
            //BIT 7,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_7(cpu, imm);
            return 12;
        });
        add_bc_op(0x7f, +[](Cpu &cpu) -> clock_t {
            //BIT 7,A
            //2  8
            //Z 0 1 -
            bit_7(cpu, cpu.m_a);
            return 8;
        });
    }
}