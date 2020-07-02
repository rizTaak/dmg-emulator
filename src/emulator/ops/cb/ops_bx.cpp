#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbbx() {
        add_bc_op(0xb0, +[](Cpu &cpu) -> clock_t {
            //RES 6,B
            //2  8
            //- - - -
            cpu.m_b = res_6(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xb1, +[](Cpu &cpu) -> clock_t {
            //RES 6,C
            //2  8
            //- - - -
            cpu.m_c = res_6(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xb2, +[](Cpu &cpu) -> clock_t {
            //RES 6,D
            //2  8
            //- - - -
            cpu.m_d = res_6(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xb3, +[](Cpu &cpu) -> clock_t {
            //RES 6,E
            //2  8
            //- - - -
            cpu.m_e = res_6(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xb4, +[](Cpu &cpu) -> clock_t {
            //RES 6,H
            //2  8
            //- - - -
            cpu.m_h = res_6(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xb5, +[](Cpu &cpu) -> clock_t {
            //RES 6,L
            //2  8
            //- - - -
            cpu.m_l = res_6(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xb6, +[](Cpu &cpu) -> clock_t {
            //RES 6,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_6(cpu, imm));
            return 16;
        });
        add_bc_op(0xb7, +[](Cpu &cpu) -> clock_t {
            //RES 6,A
            //2  8
            //- - - -
            cpu.m_a = res_6(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xb8, +[](Cpu &cpu) -> clock_t {
            //RES 7,B
            //2  8
            //- - - -
            cpu.m_b = res_7(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xb9, +[](Cpu &cpu) -> clock_t {
            //RES 7,C
            //2  8
            //- - - -
            cpu.m_c = res_7(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xba, +[](Cpu &cpu) -> clock_t {
            //RES 7,D
            //2  8
            //- - - -
            cpu.m_d = res_7(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xbb, +[](Cpu &cpu) -> clock_t {
            //RES 7,E
            //2  8
            //- - - -
            cpu.m_e = res_7(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xbc, +[](Cpu &cpu) -> clock_t {
            //RES 7,H
            //2  8
            //- - - -
            cpu.m_h = res_7(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xbd, +[](Cpu &cpu) -> clock_t {
            //RES 7,L
            //2  8
            //- - - -
            cpu.m_l = res_7(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xbe, +[](Cpu &cpu) -> clock_t {
            //RES 7,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_7(cpu, imm));
            return 16;
        });
        add_bc_op(0xbf, +[](Cpu &cpu) -> clock_t {
            //RES 7,A
            //2  8
            //- - - -
            cpu.m_a = res_7(cpu, cpu.m_a);
            return 8;
        });
    }
}