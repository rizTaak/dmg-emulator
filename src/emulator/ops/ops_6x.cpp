#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>

namespace emulator {
    void Cpu::add_6x() {
        add_op(0x60, +[](Cpu &cpu) -> clock_t {
            //LD H,B
            //1  4
            //- - - -
            cpu.m_h = cpu.m_b;
            return 4;
        });
        add_op(0x61, +[](Cpu &cpu) -> clock_t {
            //LD H,C
            //1  4
            //- - - -
            cpu.m_h = cpu.m_c;
            return 4;
        });
        add_op(0x62, +[](Cpu &cpu) -> clock_t {
            //LD H,D
            //1  4
            //- - - -
            cpu.m_h = cpu.m_d;
            return 4;
        });
        add_op(0x63, +[](Cpu &cpu) -> clock_t {
            //LD H,E
            //1  4
            //- - - -
            cpu.m_h = cpu.m_e;
            return 4;
        });
        add_op(0x64, +[](Cpu &cpu) -> clock_t {
            //LD H,H
            //1  4
            //- - - -
            cpu.m_h = cpu.m_h;
            return 4;
        });
        add_op(0x65, +[](Cpu &cpu) -> clock_t {
            //LD H,L
            //1  4
            //- - - -
            cpu.m_h = cpu.m_l;
            return 4;
        });
        add_op(0x66, +[](Cpu &cpu) -> clock_t {
            //LD H,(HL)
            //1  8
            //- - - -
            cpu.m_h = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x67, +[](Cpu &cpu) -> clock_t {
            //LD H,A
            //1  4
            //- - - -
            cpu.m_h = cpu.m_a;
            return 4;
        });
        add_op(0x68, +[](Cpu &cpu) -> clock_t {
            //LD L,B
            //1  4
            //- - - -
            cpu.m_l = cpu.m_b;
            return 4;
        });
        add_op(0x69, +[](Cpu &cpu) -> clock_t {
            //LD L,C
            //1  4
            //- - - -
            cpu.m_l = cpu.m_c;
            return 4;
        });
        add_op(0x6a, +[](Cpu &cpu) -> clock_t {
            //LD L,D
            //1  4
            //- - - -
            cpu.m_l = cpu.m_d;
            return 4;
        });
        add_op(0x6b, +[](Cpu &cpu) -> clock_t {
            //LD L,E
            //1  4
            //- - - -
            cpu.m_l = cpu.m_e;
            return 4;
        });
        add_op(0x6c, +[](Cpu &cpu) -> clock_t {
            //LD L,H
            //1  4
            //- - - -
            cpu.m_l = cpu.m_h;
            return 4;
        });
        add_op(0x6d, +[](Cpu &cpu) -> clock_t {
            //LD L,L
            //1  4
            //- - - -
            cpu.m_l = cpu.m_l;
            return 4;
        });
        add_op(0x6e, +[](Cpu &cpu) -> clock_t {
            //LD L,(HL)
            //1  8
            //- - - -
            cpu.m_l = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x6f, +[](Cpu &cpu) -> clock_t {
            //LD L,A
            //1  4
            //- - - -
            cpu.m_l = cpu.m_a;
            return 4;
        });
    }
}