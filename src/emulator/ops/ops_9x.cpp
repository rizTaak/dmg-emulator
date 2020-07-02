#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_9x() {
        add_op(0x90, +[](Cpu &cpu) -> clock_t {
            //SUB B
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0x91, +[](Cpu &cpu) -> clock_t {
            //SUB C
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0x92, +[](Cpu &cpu) -> clock_t {
            //SUB D
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0x93, +[](Cpu &cpu) -> clock_t {
            //SUB E
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0x94, +[](Cpu &cpu) -> clock_t {
            //SUB H
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0x95, +[](Cpu &cpu) -> clock_t {
            //SUB L
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0x96, +[](Cpu &cpu) -> clock_t {
            //SUB (HL)
            //1  8
            //Z 1 H C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = sub_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0x97, +[](Cpu &cpu) -> clock_t {
            //SUB A
            //1  4
            //Z 1 H C
            cpu.m_a = sub_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
        add_op(0x98, +[](Cpu &cpu) -> clock_t {
            //SBC A,B
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0x99, +[](Cpu &cpu) -> clock_t {
            //SBC A,C
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0x9a, +[](Cpu &cpu) -> clock_t {
            //SBC A,D
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0x9b, +[](Cpu &cpu) -> clock_t {
            //SBC A,E
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0x9c, +[](Cpu &cpu) -> clock_t {
            //SBC A,H
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0x9d, +[](Cpu &cpu) -> clock_t {
            //SBC A,L
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0x9e, +[](Cpu &cpu) -> clock_t {
            //SBC A,(HL)
            //1  8
            //Z 1 H C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = sbc_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0x9f, +[](Cpu &cpu) -> clock_t {
            //SBC A,A
            //1  4
            //Z 1 H C
            cpu.m_a = sbc_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
    }
}