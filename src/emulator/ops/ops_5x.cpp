#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>

namespace emulator {
    void Cpu::add_5x() {
        add_op(0x50, +[](Cpu &cpu) -> clock_t {
            //LD D,B
            //1  4
            //- - - -
            cpu.m_d = cpu.m_b;
            return 4;
        });
        add_op(0x51, +[](Cpu &cpu) -> clock_t {
            //LD D,C
            //1  4
            //- - - -
            cpu.m_d = cpu.m_c;
            return 4;
        });
        add_op(0x52, +[](Cpu &cpu) -> clock_t {
            //LD D,D
            //1  4
            //- - - -
            cpu.m_d = cpu.m_d;
            return 4;
        });
        add_op(0x53, +[](Cpu &cpu) -> clock_t {
            //LD D,E
            //1  4
            //- - - -
            cpu.m_d = cpu.m_e;
            return 4;
        });
        add_op(0x54, +[](Cpu &cpu) -> clock_t {
            //LD D,H
            //1  4
            //- - - -
            cpu.m_d = cpu.m_h;
            return 4;
        });
        add_op(0x55, +[](Cpu &cpu) -> clock_t {
            //LD D,L
            //1  4
            //- - - -
            cpu.m_d = cpu.m_l;
            return 4;
        });
        add_op(0x56, +[](Cpu &cpu) -> clock_t {
            //LD D,(HL)
            //1  8
            //- - - -
            cpu.m_d = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x57, +[](Cpu &cpu) -> clock_t {
            //LD D,A
            //1  4
            //- - - -
            cpu.m_d = cpu.m_a;
            return 4;
        });
        add_op(0x58, +[](Cpu &cpu) -> clock_t {
            //LD E,B
            //1  4
            //- - - -
            cpu.m_e = cpu.m_b;
            return 4;
        });
        add_op(0x59, +[](Cpu &cpu) -> clock_t {
            //LD E,C
            //1  4
            //- - - -
            cpu.m_e = cpu.m_c;
            return 4;
        });
        add_op(0x5a, +[](Cpu &cpu) -> clock_t {
            //LD E,D
            //1  4
            //- - - -
            cpu.m_e = cpu.m_d;
            return 4;
        });
        add_op(0x5b, +[](Cpu &cpu) -> clock_t {
            //LD E,E
            //1  4
            //- - - -
            cpu.m_e = cpu.m_e;
            return 4;
        });
        add_op(0x5c, +[](Cpu &cpu) -> clock_t {
            //LD E,H
            //1  4
            //- - - -
            cpu.m_e = cpu.m_h;
            return 4;
        });
        add_op(0x5d, +[](Cpu &cpu) -> clock_t {
            //LD E,L
            //1  4
            //- - - -
            cpu.m_e = cpu.m_l;
            return 4;
        });
        add_op(0x5e, +[](Cpu &cpu) -> clock_t {
            //LD E,(HL)
            //1  8
            //- - - -
            cpu.m_e = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x5f, +[](Cpu &cpu) -> clock_t {
            //LD E,A
            //1  4
            //- - - -
            cpu.m_e = cpu.m_a;
            return 4;
        });
    }
}