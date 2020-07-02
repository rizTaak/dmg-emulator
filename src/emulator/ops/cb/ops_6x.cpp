#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb6x() {
        add_bc_op(0x60, +[](Cpu &cpu) -> clock_t {
            //BIT 4,B
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x61, +[](Cpu &cpu) -> clock_t {
            //BIT 4,C
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x62, +[](Cpu &cpu) -> clock_t {
            //BIT 4,D
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x63, +[](Cpu &cpu) -> clock_t {
            //BIT 4,E
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x64, +[](Cpu &cpu) -> clock_t {
            //BIT 4,H
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x65, +[](Cpu &cpu) -> clock_t {
            //BIT 4,L
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x66, +[](Cpu &cpu) -> clock_t {
            //BIT 4,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_4(cpu, imm);
            return 12;
        });
        add_bc_op(0x67, +[](Cpu &cpu) -> clock_t {
            //BIT 4,A
            //2  8
            //Z 0 1 -
            bit_4(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x68, +[](Cpu &cpu) -> clock_t {
            //BIT 5,B
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x69, +[](Cpu &cpu) -> clock_t {
            //BIT 5,C
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x6a, +[](Cpu &cpu) -> clock_t {
            //BIT 5,D
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x6b, +[](Cpu &cpu) -> clock_t {
            //BIT 5,E
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x6c, +[](Cpu &cpu) -> clock_t {
            //BIT 5,H
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x6d, +[](Cpu &cpu) -> clock_t {
            //BIT 5,L
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x6e, +[](Cpu &cpu) -> clock_t {
            //BIT 5,(HL)
            //2  16
            //Z 0 1 -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            bit_5(cpu, imm);
            return 12;
        });
        add_bc_op(0x6f, +[](Cpu &cpu) -> clock_t {
            //BIT 5,A
            //2  8
            //Z 0 1 -
            bit_5(cpu, cpu.m_a);
            return 8;
        });
    }
}