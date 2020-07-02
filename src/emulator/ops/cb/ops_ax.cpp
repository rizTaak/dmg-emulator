#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cbax() {
        add_bc_op(0xa0, +[](Cpu &cpu) -> clock_t {
            //RES 4,B
            //2  8
            //- - - -
            cpu.m_b = res_4(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xa1, +[](Cpu &cpu) -> clock_t {
            //RES 4,C
            //2  8
            //- - - -
            cpu.m_c = res_4(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xa2, +[](Cpu &cpu) -> clock_t {
            //RES 4,D
            //2  8
            //- - - -
            cpu.m_d = res_4(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xa3, +[](Cpu &cpu) -> clock_t {
            //RES 4,E
            //2  8
            //- - - -
            cpu.m_e = res_4(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xa4, +[](Cpu &cpu) -> clock_t {
            //RES 4,H
            //2  8
            //- - - -
            cpu.m_h = res_4(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xa5, +[](Cpu &cpu) -> clock_t {
            //RES 4,L
            //2  8
            //- - - -
            cpu.m_l = res_4(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xa6, +[](Cpu &cpu) -> clock_t {
            //RES 4,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_4(cpu, imm));
            return 16;
        });
        add_bc_op(0xa7, +[](Cpu &cpu) -> clock_t {
            //RES 4,A
            //2  8
            //- - - -
            cpu.m_a = res_4(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0xa8, +[](Cpu &cpu) -> clock_t {
            //RES 5,B
            //2  8
            //- - - -
            cpu.m_b = res_5(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0xa9, +[](Cpu &cpu) -> clock_t {
            //RES 5,C
            //2  8
            //- - - -
            cpu.m_c = res_5(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0xaa, +[](Cpu &cpu) -> clock_t {
            //RES 5,D
            //2  8
            //- - - -
            cpu.m_d = res_5(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0xab, +[](Cpu &cpu) -> clock_t {
            //RES 5,E
            //2  8
            //- - - -
            cpu.m_e = res_5(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0xac, +[](Cpu &cpu) -> clock_t {
            //RES 5,H
            //2  8
            //- - - -
            cpu.m_h = res_5(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0xad, +[](Cpu &cpu) -> clock_t {
            //RES 5,L
            //2  8
            //- - - -
            cpu.m_l = res_5(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0xae, +[](Cpu &cpu) -> clock_t {
            //RES 5,(HL)
            //2  16
            //- - - -
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, res_5(cpu, imm));
            return 16;
        });
        add_bc_op(0xaf, +[](Cpu &cpu) -> clock_t {
            //RES 5,A
            //2  8
            //- - - -
            cpu.m_a = res_5(cpu, cpu.m_a);
            return 8;
        });
    }
}