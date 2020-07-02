#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_2x() {
        add_op(0x20, +[](Cpu &cpu) -> clock_t {
            //JR NZ,r8
            //2  12/8
            //- - - -
            auto jump = static_cast<int8_t>(cpu.fetch());
            if(!cpu.z_flag_zero()) {
                cpu.m_pc += jump;
                return 12;
            } else {
                return 8;
            }
        });
        add_op(0x21, +[](Cpu &cpu) -> clock_t {
            //LD HL,d16
            //3  12
            //- - - -
            cpu.m_hl = cpu.wfetch();
            return 12;
        });
        add_op(0x22, +[](Cpu &cpu) -> clock_t {
            //LD (HL+),A
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_a);
            cpu.m_hl++;
            return 8;
        });
        add_op(0x23, +[](Cpu &cpu) -> clock_t {
            //INC HL
            //1  8
            //- - - -
            cpu.m_hl++;
            return 8;
        });
        add_op(0x24, +[](Cpu &cpu) -> clock_t {
            //INC H
            //1  4
            //Z 0 H -
            cpu.m_h = inc_n(cpu, cpu.m_h);
            return 4;
        });
        add_op(0x25, +[](Cpu &cpu) -> clock_t {
            //DEC H
            //1  4
            //Z 1 H -
            cpu.m_h = dec_n(cpu, cpu.m_h);
            return 4;
        });
        add_op(0x26, +[](Cpu &cpu) -> clock_t {
            //LD H,d8
            //2  8
            //- - - -
            auto imm = cpu.fetch();
            cpu.m_h = imm;
            return 8;
        });
        add_op(0x27, +[](Cpu &cpu) -> clock_t {
            //DAA
            //1  4
            //Z - 0 C
            // from gbemu
            auto reg = cpu.m_a;
            register_16_t correction = cpu.c_flag_value() ? 0x60 : 0x00;
            if (cpu.h_flag_value() || (!cpu.n_flag_value() && ((reg & 0x0F) > 9))) {
                correction |= 0x06;
            }
            if (cpu.c_flag_value() || (!cpu.n_flag_value() && (reg > 0x99))) {
                correction |= 0x60;
            }
            if (cpu.n_flag_value()) {
                reg = static_cast<register_8_t>(reg - correction);
            } else {
                reg = static_cast<register_8_t>(reg + correction);
            }
            if (((correction << 2) & 0x100) != 0) {
                cpu.update_c_flag(true);
            }
            cpu.update_h_flag(false);
            cpu.update_z_flag(reg);
            cpu.m_a = static_cast<register_8_t>(reg);
            return 4;
        });
        add_op(0x28, +[](Cpu &cpu) -> clock_t {
            //JR Z,r8
            //2  12/8
            //- - - -
            auto imm = static_cast<int8_t>(cpu.fetch());
            if (cpu.z_flag_zero()) {
                cpu.m_pc += imm;
                return 12;
            } else {
                return 8;
            }
        });
        add_op(0x29, +[](Cpu &cpu) -> clock_t {
            //ADD HL,HL
            //1  8
            //- 0 H C
            cpu.m_hl = add_nn(cpu, cpu.m_hl, cpu.m_hl);
            return 8;
        });
        add_op(0x2a, +[](Cpu &cpu) -> clock_t {
            //LD A,(HL+)
            //1  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_hl++;
            return 8;
        });
        add_op(0x2b, +[](Cpu &cpu) -> clock_t {
            //DEC HL
            //1  8
            //- - - -
            cpu.m_hl--;
            return 8;
        });
        add_op(0x2c, +[](Cpu &cpu) -> clock_t {
            //INC L
            //1  4
            //Z 0 H -
            cpu.m_l = inc_n(cpu, cpu.m_l);
            return 4;
        });
        add_op(0x2d, +[](Cpu &cpu) -> clock_t {
            //DEC L
            //1  4
            //Z 1 H -
            cpu.m_l = dec_n(cpu, cpu.m_l);
            return 4;
        });
        add_op(0x2e, +[](Cpu &cpu) -> clock_t {
            //LD L,d8
            //2  8
            //- - - -
            auto imm = cpu.fetch();
            cpu.m_l = imm;
            return 8;
        });
        add_op(0x2f, +[](Cpu &cpu) -> clock_t {
            //CPL
            //1  4
            //- 1 1 -
            cpu.m_a = ~cpu.m_a;
            cpu.set_n_flag();
            cpu.set_h_flag();
            return 4;
        });
    }
}