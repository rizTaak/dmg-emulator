#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbex() {
        add_bc_op(0xe0, +[](Cpu &cpu) -> clock_t {
            //SET 4,B
            //2  8
            //- - - -
            cpu.m_b = set_4(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xe1, +[](Cpu &cpu) -> clock_t {
            //SET 4,C
            //2  8
            //- - - -
            cpu.m_c = set_4(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xe2, +[](Cpu &cpu) -> clock_t {
            //SET 4,D
            //2  8
            //- - - -
            cpu.m_d = set_4(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xe3, +[](Cpu &cpu) -> clock_t {
            //SET 4,E
            //2  8
            //- - - -
            cpu.m_e = set_4(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xe4, +[](Cpu &cpu) -> clock_t {
            //SET 4,H
            //2  8
            //- - - -
            cpu.m_h = set_4(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xe5, +[](Cpu &cpu) -> clock_t {
            //SET 4,L
            //2  8
            //- - - -
            cpu.m_l = set_4(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xe6, +[](Cpu &cpu) -> clock_t {
            //SET 4,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_4(cpu, imm));
            return 16;
        });
        add_bc_op(0xe7, +[](Cpu &cpu) -> clock_t {
            //SET 4,A
            //2  8
            //- - - -
            cpu.m_a = set_4(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xe8, +[](Cpu &cpu) -> clock_t {
            //SET 5,B
            //2  8
            //- - - -
            cpu.m_b = set_5(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xe9, +[](Cpu &cpu) -> clock_t {
            //SET 5,C
            //2  8
            //- - - -
            cpu.m_c = set_5(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xea, +[](Cpu &cpu) -> clock_t {
            //SET 5,D
            //2  8
            //- - - -
            cpu.m_d = set_5(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xeb, +[](Cpu &cpu) -> clock_t {
            //SET 5,E
            //2  8
            //- - - -
            cpu.m_e = set_5(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xec, +[](Cpu &cpu) -> clock_t {
            //SET 5,H
            //2  8
            //- - - -
            cpu.m_h = set_5(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xed, +[](Cpu &cpu) -> clock_t {
            //SET 5,L
            //2  8
            //- - - -
            cpu.m_l = set_5(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xee, +[](Cpu &cpu) -> clock_t {
            //SET 5,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_5(cpu, imm));
            return 16;
        });
        add_bc_op(0xef, +[](Cpu &cpu) -> clock_t {
            //SET 5,A
            //2  8
            //- - - -
            cpu.m_a = set_5(cpu, cpu.m_a);
            return 8;
        });
    }
}