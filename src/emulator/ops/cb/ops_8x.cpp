#include <emulator/cpu.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb8x() {
        add_bc_op(0x80, +[](Cpu &cpu) -> clock_t {
            //RES 0,B
            //2  8
            //- - - -
            cpu.m_b = res_0(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x81, +[](Cpu &cpu) -> clock_t {
            //RES 0,C
            //2  8
            //- - - -
            cpu.m_c = res_0(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x82, +[](Cpu &cpu) -> clock_t {
            //RES 0,D
            //2  8
            //- - - -
            cpu.m_d = res_0(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x83, +[](Cpu &cpu) -> clock_t {
            //RES 0,E
            //2  8
            //- - - -
            cpu.m_e = res_0(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x84, +[](Cpu &cpu) -> clock_t {
            //RES 0,H
            //2  8
            //- - - -
            cpu.m_h = res_0(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x85, +[](Cpu &cpu) -> clock_t {
            //RES 0,L
            //2  8
            //- - - -
            cpu.m_l = res_0(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x86, +[](Cpu &cpu) -> clock_t {
            //RES 0,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_0(cpu, imm));
            return 16;
        });
        add_bc_op(0x87, +[](Cpu &cpu) -> clock_t {
            //RES 0,A
            //2  8
            //- - - -
            cpu.m_a = res_0(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x88, +[](Cpu &cpu) -> clock_t {
            //RES 1,B
            //2  8
            //- - - -
            cpu.m_b = res_1(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x89, +[](Cpu &cpu) -> clock_t {
            //RES 1,C
            //2  8
            //- - - -
            cpu.m_c = res_1(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x8a, +[](Cpu &cpu) -> clock_t {
            //RES 1,D
            //2  8
            //- - - -
            cpu.m_d = res_1(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x8b, +[](Cpu &cpu) -> clock_t {
            //RES 1,E
            //2  8
            //- - - -
            cpu.m_e = res_1(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x8c, +[](Cpu &cpu) -> clock_t {
            //RES 1,H
            //2  8
            //- - - -
            cpu.m_h = res_1(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x8d, +[](Cpu &cpu) -> clock_t {
            //RES 1,L
            //2  8
            //- - - -
            cpu.m_l = res_1(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x8e, +[](Cpu &cpu) -> clock_t {
            //RES 1,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_1(cpu, imm));
            return 16;
        });
        add_bc_op(0x8f, +[](Cpu &cpu) -> clock_t {
            //RES 1,A
            //2  8
            //- - - -
            cpu.m_a = res_1(cpu, cpu.m_a);
            return 8;
        });
    }
}