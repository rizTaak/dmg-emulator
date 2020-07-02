#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>

namespace emulator {
    void Cpu::add_7x() {
        add_op(0x70, +[](Cpu &cpu) -> clock_t {
            //LD (HL),B
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_b);
            return 8;
        });
        add_op(0x71, +[](Cpu &cpu) -> clock_t {
            //LD (HL),C
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_c);
            return 8;
        });
        add_op(0x72, +[](Cpu &cpu) -> clock_t {
            //LD (HL),D
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_d);
            return 8;
        });
        add_op(0x73, +[](Cpu &cpu) -> clock_t {
            //LD (HL),E
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_e);
            return 8;
        });
        add_op(0x74, +[](Cpu &cpu) -> clock_t {
            //LD (HL),H
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_h);
            return 8;
        });
        add_op(0x75, +[](Cpu &cpu) -> clock_t {
            //LD (HL),L
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_l);
            return 8;
        });
        add_op(0x76, +[](Cpu &cpu) -> clock_t {
            //HALT
            //1  4
            //- - - -
            cpu.halt();
            return 4;
        });
        add_op(0x77, +[](Cpu &cpu) -> clock_t {
            //LD (HL),A
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_a);
            return 8;
        });
        add_op(0x78, +[](Cpu &cpu) -> clock_t {
            //LD A,B
            //1  4
            //- - - -
            cpu.m_a = cpu.m_b;
            return 4;
        });
        add_op(0x79, +[](Cpu &cpu) -> clock_t {
            //LD A,C
            //1  4
            //- - - -
            cpu.m_a = cpu.m_c;
            return 4;
        });
        add_op(0x7a, +[](Cpu &cpu) -> clock_t {
            //LD A,D
            //1  4
            //- - - -
            cpu.m_a = cpu.m_d;
            return 4;
        });
        add_op(0x7b, +[](Cpu &cpu) -> clock_t {
            //LD A,E
            //1  4
            //- - - -
            cpu.m_a = cpu.m_e;
            return 4;
        });
        add_op(0x7c, +[](Cpu &cpu) -> clock_t {
            //LD A,H
            //1  4
            //- - - -
            cpu.m_a = cpu.m_h;
            return 4;
        });
        add_op(0x7d, +[](Cpu &cpu) -> clock_t {
            //LD A,L
            //1  4
            //- - - -
            cpu.m_a = cpu.m_l;
            return 4;
        });
        add_op(0x7e, +[](Cpu &cpu) -> clock_t {
            //LD A,(HL)
            //1  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x7f, +[](Cpu &cpu) -> clock_t {
            //LD A,A
            //1  4
            //- - - -
            cpu.m_a = cpu.m_a;
            return 4;
        });
    }
}