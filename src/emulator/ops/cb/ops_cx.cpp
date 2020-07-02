#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbcx() {
        add_bc_op(0xc0, +[](Cpu &cpu) -> clock_t {
            //SET 0,B
            //2  8
            //- - - -
            cpu.m_b = set_0(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xc1, +[](Cpu &cpu) -> clock_t {
            //SET 0,C
            //2  8
            //- - - -
            cpu.m_c = set_0(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xc2, +[](Cpu &cpu) -> clock_t {
            //SET 0,D
            //2  8
            //- - - -
            cpu.m_d = set_0(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xc3, +[](Cpu &cpu) -> clock_t {
            //SET 0,E
            //2  8
            //- - - -
            cpu.m_e = set_0(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xc4, +[](Cpu &cpu) -> clock_t {
            //SET 0,H
            //2  8
            //- - - -
            cpu.m_h = set_0(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xc5, +[](Cpu &cpu) -> clock_t {
            //SET 0,L
            //2  8
            //- - - -
            cpu.m_l = set_0(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xc6, +[](Cpu &cpu) -> clock_t {
            //SET 0,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_0(cpu, imm));
            return 16;
        });
        add_bc_op(0xc7, +[](Cpu &cpu) -> clock_t {
            //SET 0,A
            //2  8
            //- - - -
            cpu.m_a = set_0(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xc8, +[](Cpu &cpu) -> clock_t {
            //SET 1,B
            //2  8
            //- - - -
            cpu.m_b = set_1(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xc9, +[](Cpu &cpu) -> clock_t {
            //SET 1,C
            //2  8
            //- - - -
            cpu.m_c = set_1(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xca, +[](Cpu &cpu) -> clock_t {
            //SET 1,D
            //2  8
            //- - - -
            cpu.m_d = set_1(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xcb, +[](Cpu &cpu) -> clock_t {
            //SET 1,E
            //2  8
            //- - - -
            cpu.m_e = set_1(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xcc, +[](Cpu &cpu) -> clock_t {
            //SET 1,H
            //2  8
            //- - - -
            cpu.m_h = set_1(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xcd, +[](Cpu &cpu) -> clock_t {
            //SET 1,L
            //2  8
            //- - - -
            cpu.m_l = set_1(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xce, +[](Cpu &cpu) -> clock_t {
            //SET 1,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_1(cpu, imm));
            return 16;
        });
        add_bc_op(0xcf, +[](Cpu &cpu) -> clock_t {
            //SET 1,A
            //2  8
            //- - - -
            cpu.m_a = set_1(cpu, cpu.m_a);
            return 8;
        });
    }
}