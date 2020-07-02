#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb9x() {
        add_bc_op(0x90, +[](Cpu &cpu) -> clock_t {
            //RES 2,B
            //2  8
            //- - - -
            cpu.m_b = res_2(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x91, +[](Cpu &cpu) -> clock_t {
            //RES 2,C
            //2  8
            //- - - -
            cpu.m_c = res_2(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x92, +[](Cpu &cpu) -> clock_t {
            //RES 2,D
            //2  8
            //- - - -
            cpu.m_d = res_2(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x93, +[](Cpu &cpu) -> clock_t {
            //RES 2,E
            //2  8
            //- - - -
            cpu.m_e = res_2(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x94, +[](Cpu &cpu) -> clock_t {
            //RES 2,H
            //2  8
            //- - - -
            cpu.m_h = res_2(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x95, +[](Cpu &cpu) -> clock_t {
            //RES 2,L
            //2  8
            //- - - -
            cpu.m_l = res_2(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x96, +[](Cpu &cpu) -> clock_t {
            //RES 2,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_2(cpu, imm));
            return 16;
        });
        add_bc_op(0x97, +[](Cpu &cpu) -> clock_t {
            //RES 2,A
            //2  8
            //- - - -
            cpu.m_a = res_2(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x98, +[](Cpu &cpu) -> clock_t {
            //RES 3,B
            //2  8
            //- - - -
            cpu.m_b = res_3(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x99, +[](Cpu &cpu) -> clock_t {
            //RES 3,C
            //2  8
            //- - - -
            cpu.m_c = res_3(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x9a, +[](Cpu &cpu) -> clock_t {
            //RES 3,D
            //2  8
            //- - - -
            cpu.m_d = res_3(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x9b, +[](Cpu &cpu) -> clock_t {
            //RES 3,E
            //2  8
            //- - - -
            cpu.m_e = res_3(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x9c, +[](Cpu &cpu) -> clock_t {
            //RES 3,H
            //2  8
            //- - - -
            cpu.m_h = res_3(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x9d, +[](Cpu &cpu) -> clock_t {
            //RES 3,L
            //2  8
            //- - - -
            cpu.m_l = res_3(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x9e, +[](Cpu &cpu) -> clock_t {
            //RES 3,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_3(cpu, imm));
            return 16;
        });
        add_bc_op(0x9f, +[](Cpu &cpu) -> clock_t {
            //RES 3,A
            //2  8
            //- - - -
            cpu.m_a = res_3(cpu, cpu.m_a);
            return 8;
        });
    }
}