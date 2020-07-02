#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cb2x() {
        add_bc_op(0x20, +[](Cpu &cpu) -> clock_t {
            //SLA B
            //2  8
            //Z 0 0 C
            cpu.m_b = sla_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x21, +[](Cpu &cpu) -> clock_t {
            //SLA C
            //2  8
            //Z 0 0 C
            cpu.m_c = sla_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x22, +[](Cpu &cpu) -> clock_t {
            //SLA D
            //2  8
            //Z 0 0 C
            cpu.m_d = sla_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x23, +[](Cpu &cpu) -> clock_t {
            //SLA E
            //2  8
            //Z 0 0 C
            cpu.m_e = sla_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x24, +[](Cpu &cpu) -> clock_t {
            //SLA H
            //2  8
            //Z 0 0 C
            cpu.m_h = sla_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x25, +[](Cpu &cpu) -> clock_t {
            //SLA L
            //2  8
            //Z 0 0 C
            cpu.m_l = sla_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x26, +[](Cpu &cpu) -> clock_t {
            //SLA (HL)
            //2  16
            //Z 0 0 C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, sla_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x27, +[](Cpu &cpu) -> clock_t {
            //SLA A
            //2  8
            //Z 0 0 C
            cpu.m_a = sla_n(cpu, cpu.m_a);
            return 8;
        });
        add_bc_op(0x28, +[](Cpu &cpu) -> clock_t {
            //todo: sra n document for C flag is contradictory
            //SRA B
            //2  8
            //Z 0 0 0
            cpu.m_b = sra_n(cpu, cpu.m_b);
            return 8;
        });
        add_bc_op(0x29, +[](Cpu &cpu) -> clock_t {
            //SRA C
            //2  8
            //Z 0 0 0
            cpu.m_c = sra_n(cpu, cpu.m_c);
            return 8;
        });
        add_bc_op(0x2a, +[](Cpu &cpu) -> clock_t {
            //SRA D
            //2  8
            //Z 0 0 0
            cpu.m_d = sra_n(cpu, cpu.m_d);
            return 8;
        });
        add_bc_op(0x2b, +[](Cpu &cpu) -> clock_t {
            //SRA E
            //2  8
            //Z 0 0 0
            cpu.m_e = sra_n(cpu, cpu.m_e);
            return 8;
        });
        add_bc_op(0x2c, +[](Cpu &cpu) -> clock_t {
            //SRA H
            //2  8
            //Z 0 0 0
            cpu.m_h = sra_n(cpu, cpu.m_h);
            return 8;
        });
        add_bc_op(0x2d, +[](Cpu &cpu) -> clock_t {
            //SRA L
            //2  8
            //Z 0 0 0
            cpu.m_l = sra_n(cpu, cpu.m_l);
            return 8;
        });
        add_bc_op(0x2e, +[](Cpu &cpu) -> clock_t {
            //SRA (HL)
            //2  16
            //Z 0 0 0
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_mem.write_byte(cpu.m_hl, sra_n(cpu, imm));
            return 16;
        });
        add_bc_op(0x2f, +[](Cpu &cpu) -> clock_t {
            //SRA A
            //2  8
            //Z 0 0 0
            cpu.m_a = sra_n(cpu, cpu.m_a);
            return 8;
        });
    }
}