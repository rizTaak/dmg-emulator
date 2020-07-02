#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbfx() {
        add_bc_op(0xf0, +[](Cpu &cpu) -> clock_t {
            //SET 6,B
            //2  8
            //- - - -
            cpu.m_b = set_6(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xf1, +[](Cpu &cpu) -> clock_t {
            //SET 6,C
            //2  8
            //- - - -
            cpu.m_c = set_6(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xf2, +[](Cpu &cpu) -> clock_t {
            //SET 6,D
            //2  8
            //- - - -
            cpu.m_d = set_6(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xf3, +[](Cpu &cpu) -> clock_t {
            //SET 6,E
            //2  8
            //- - - -
            cpu.m_e = set_6(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xf4, +[](Cpu &cpu) -> clock_t {
            //SET 6,H
            //2  8
            //- - - -
            cpu.m_h = set_6(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xf5, +[](Cpu &cpu) -> clock_t {
            //SET 6,L
            //2  8
            //- - - -
            cpu.m_l = set_6(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xf6, +[](Cpu &cpu) -> clock_t {
            //SET 6,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_6(cpu, imm));
            return 16;
        });
        add_bc_op(0xf7, +[](Cpu &cpu) -> clock_t {
            //SET 6,A
            //2  8
            //- - - -
            cpu.m_a = set_6(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xf8, +[](Cpu &cpu) -> clock_t {
            //SET 7,B
            //2  8
            //- - - -
            cpu.m_b = set_7(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xf9, +[](Cpu &cpu) -> clock_t {
            //SET 7,C
            //2  8
            //- - - -
            cpu.m_c = set_7(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xfa, +[](Cpu &cpu) -> clock_t {
            //SET 7,D
            //2  8
            //- - - -
            cpu.m_d = set_7(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xfb, +[](Cpu &cpu) -> clock_t {
            //SET 7,E
            //2  8
            //- - - -
            cpu.m_e = set_7(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xfc, +[](Cpu &cpu) -> clock_t {
            //SET 7,H
            //2  8
            //- - - -
            cpu.m_h = set_7(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xfd, +[](Cpu &cpu) -> clock_t {
            //SET 7,L
            //2  8
            //- - - -
            cpu.m_l = set_7(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xfe, +[](Cpu &cpu) -> clock_t {
            //SET 7,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_7(cpu, imm));
            return 16;
        });
        add_bc_op(0xff, +[](Cpu &cpu) -> clock_t {
            //SET 7,A
            //2  8
            //- - - -
            cpu.m_a = set_7(cpu, cpu.m_a);
            return 8;
        });
    }
}