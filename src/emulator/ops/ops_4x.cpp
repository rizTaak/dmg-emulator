#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>

namespace emulator {
    void Cpu::add_4x() {
        add_op(0x40, +[](Cpu &cpu) -> clock_t {
            //LD B,B
            //1  4
            //- - - -
            cpu.m_b = cpu.m_b;
            return 4;
        });
        add_op(0x41, +[](Cpu &cpu) -> clock_t {
            //LD B,C
            //1  4
            //- - - -
            cpu.m_b = cpu.m_c;
            return 4;
        });
        add_op(0x42, +[](Cpu &cpu) -> clock_t {
            //LD B,D
            //1  4
            //- - - -
            cpu.m_b = cpu.m_d;
            return 4;
        });
        add_op(0x43, +[](Cpu &cpu) -> clock_t {
            //LD B,E
            //1  4
            //- - - -
            cpu.m_b = cpu.m_e;
            return 4;
        });
        add_op(0x44, +[](Cpu &cpu) -> clock_t {
            //LD B,H
            //1  4
            //- - - -
            cpu.m_b = cpu.m_h;
            return 4;
        });
        add_op(0x45, +[](Cpu &cpu) -> clock_t {
            //LD B,L
            //1  4
            //- - - -
            cpu.m_b = cpu.m_l;
            return 4;
        });
        add_op(0x46, +[](Cpu &cpu) -> clock_t {
            //LD B,(HL)
            //1  8
            //- - - -
            cpu.m_b = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x47, +[](Cpu &cpu) -> clock_t {
            //LD B,A
            //1  4
            //- - - -
            cpu.m_b = cpu.m_a;
            return 4;
        });
        add_op(0x48, +[](Cpu &cpu) -> clock_t {
            //LD C,B
            //1  4
            //- - - -
            cpu.m_c = cpu.m_b;
            return 4;
        });
        add_op(0x49, +[](Cpu &cpu) -> clock_t {
            //LD C,C
            //1  4
            //- - - -
            cpu.m_c = cpu.m_c;
            return 4;
        });
        add_op(0x4a, +[](Cpu &cpu) -> clock_t {
            //LD C,D
            //1  4
            //- - - -
            cpu.m_c = cpu.m_d;
            return 4;
        });
        add_op(0x4b, +[](Cpu &cpu) -> clock_t {
            //LD C,E
            //1  4
            //- - - -
            cpu.m_c = cpu.m_e;
            return 4;
        });
        add_op(0x4c, +[](Cpu &cpu) -> clock_t {
            //LD C,H
            //1  4
            //- - - -
            cpu.m_c = cpu.m_h;
            return 4;
        });
        add_op(0x4d, +[](Cpu &cpu) -> clock_t {
            //LD C,L
            //1  4
            //- - - -
            cpu.m_c = cpu.m_l;
            return 4;
        });
        add_op(0x4e, +[](Cpu &cpu) -> clock_t {
            //LD C,(HL)
            //1  8
            //- - - -
            cpu.m_c = cpu.m_mem.read_byte(cpu.m_hl);
            return 8;
        });
        add_op(0x4f, +[](Cpu &cpu) -> clock_t {
            //LD C,A
            //1  4
            //- - - -
            cpu.m_c = cpu.m_a;
            return 4;
        });
    }
}