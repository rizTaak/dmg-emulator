#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbdx() {
        add_bc_op(0xd0, +[](Cpu &cpu) -> clock_t {
            //SET 2,B
            //2  8
            //- - - -
            cpu.m_b = set_2(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xd1, +[](Cpu &cpu) -> clock_t {
            //SET 2,C
            //2  8
            //- - - -
            cpu.m_c = set_2(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xd2, +[](Cpu &cpu) -> clock_t {
            //SET 2,D
            //2  8
            //- - - -
            cpu.m_d = set_2(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xd3, +[](Cpu &cpu) -> clock_t {
            //SET 2,E
            //2  8
            //- - - -
            cpu.m_e = set_2(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xd4, +[](Cpu &cpu) -> clock_t {
            //SET 2,H
            //2  8
            //- - - -
            cpu.m_h = set_2(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xd5, +[](Cpu &cpu) -> clock_t {
            //SET 2,L
            //2  8
            //- - - -
            cpu.m_l = set_2(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xd6, +[](Cpu &cpu) -> clock_t {
            //SET 2,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_2(cpu, imm));
            return 16;
        });
        add_bc_op(0xd7, +[](Cpu &cpu) -> clock_t {
            //SET 2,A
            //2  8
            //- - - -
            cpu.m_a = set_2(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xd8, +[](Cpu &cpu) -> clock_t {
            //SET 3,B
            //2  8
            //- - - -
            cpu.m_b = set_3(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xd9, +[](Cpu &cpu) -> clock_t {
            //SET 3,C
            //2  8
            //- - - -
            cpu.m_c = set_3(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xda, +[](Cpu &cpu) -> clock_t {
            //SET 3,D
            //2  8
            //- - - -
            cpu.m_d = set_3(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xdb, +[](Cpu &cpu) -> clock_t {
            //SET 3,E
            //2  8
            //- - - -
            cpu.m_e = set_3(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xdc, +[](Cpu &cpu) -> clock_t {
            //SET 3,H
            //2  8
            //- - - -
            cpu.m_h = set_3(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xdd, +[](Cpu &cpu) -> clock_t {
            //SET 3,L
            //2  8
            //- - - -
            cpu.m_l = set_3(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xde, +[](Cpu &cpu) -> clock_t {
            //SET 3,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, set_3(cpu, imm));
            return 16;
        });
        add_bc_op(0xdf, +[](Cpu &cpu) -> clock_t {
            //SET 3,A
            //2  8
            //- - - -
            cpu.m_a = set_3(cpu, cpu.m_a);
            return 8;
        });
    }
}