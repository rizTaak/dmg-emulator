#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb0x() {
        add_bc_op(0x00, +[](Cpu &cpu) -> clock_t {
            //RLC B
            //2  8
            //Z 0 0 C
            cpu.m_b = rlc_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x01, +[](Cpu &cpu) -> clock_t {
            //RLC C
            //2  8
            //Z 0 0 C
            cpu.m_c = rlc_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x02, +[](Cpu &cpu) -> clock_t {
            //RLC D
            //2  8
            //Z 0 0 C
            cpu.m_d = rlc_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x03, +[](Cpu &cpu) -> clock_t {
            //RLC E
            //2  8
            //Z 0 0 C
            cpu.m_e = rlc_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x04, +[](Cpu &cpu) -> clock_t {
            //RLC H
            //2  8
            //Z 0 0 C
            cpu.m_h = rlc_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x05, +[](Cpu &cpu) -> clock_t {
            //RLC L
            //2  8
            //Z 0 0 C
            cpu.m_l = rlc_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x06, +[](Cpu &cpu) -> clock_t {
            //RLC (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, rlc_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x07, +[](Cpu &cpu) -> clock_t {
            //RLC A
            //2  8
            //Z 0 0 C
            cpu.m_a = rlc_n(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x08, +[](Cpu &cpu) -> clock_t {
            //RRC B
            //2  8
            //Z 0 0 C
            cpu.m_b = rrc_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x09, +[](Cpu &cpu) -> clock_t {
            //RRC C
            //2  8
            //Z 0 0 C
            cpu.m_c = rrc_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x0a, +[](Cpu &cpu) -> clock_t {
            //RRC D
            //2  8
            //Z 0 0 C
            cpu.m_d = rrc_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x0b, +[](Cpu &cpu) -> clock_t {
            //RRC E
            //2  8
            //Z 0 0 C
            cpu.m_e = rrc_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x0c, +[](Cpu &cpu) -> clock_t {
            //RRC H
            //2  8
            //Z 0 0 C
            cpu.m_h = rrc_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x0d, +[](Cpu &cpu) -> clock_t {
            //RRC L
            //2  8
            //Z 0 0 C
            cpu.m_l = rrc_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x0e, +[](Cpu &cpu) -> clock_t {
            //RRC (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, rrc_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x0f, +[](Cpu &cpu) -> clock_t {
            //RRC A
            //2  8
            //Z 0 0 C
            cpu.m_a = rrc_n(cpu, cpu.m_a);
            return 8;
        });
    }
}