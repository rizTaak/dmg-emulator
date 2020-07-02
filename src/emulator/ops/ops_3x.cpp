#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_3x() {
        add_op(0x30, +[](Cpu &cpu) -> clock_t {
            //JR NC,r8
            //2  12/8
            //- - - -
            int8_t imm = cpu.fetch();
            if(cpu.c_flag_value()) {
                return 8;
            } else {
                cpu.m_pc += imm;
                return 12;
            }
        });
        add_op(0x31, +[](Cpu &cpu) -> clock_t {
            //LD SP,d16
            //3  12
            //- - - -
            cpu.m_sp = cpu.wfetch();
            return 12;
        });
        add_op(0x32, +[](Cpu &cpu) -> clock_t {
            //LD (HL-),A
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_hl, cpu.m_a);
            cpu.m_hl--;
            return 8;
        });
        add_op(0x33, +[](Cpu &cpu) -> clock_t {
            //INC SP
            //1  8
            //- - - -
            cpu.m_sp++;
            return 8;
        });
        add_op(0x34, +[](Cpu &cpu) -> clock_t {
            //INC (HL)
            //1  12
            //Z 0 H -
            auto value = cpu.m_mem.read_byte(cpu.m_hl);
            value = inc_n(cpu, value);
            cpu.m_mem.write_byte(cpu.m_hl, value);
            return 12;
        });
        add_op(0x35, +[](Cpu &cpu) -> clock_t {
            //DEC (HL)
            //1  12
            //Z 1 H -
            auto value = cpu.m_mem.read_byte(cpu.m_hl);
            value = dec_n(cpu, value);
            cpu.m_mem.write_byte(cpu.m_hl, value);
            return 12;
        });
        add_op(0x36, +[](Cpu &cpu) -> clock_t {
            //LD (HL),d8
            //2  12
            //- - - -
            auto imm = cpu.fetch();
            cpu.m_mem.write_byte(cpu.m_hl, imm);
            return 12;
        });
        add_op(0x37, +[](Cpu &cpu) -> clock_t {
            //SCF
            //1  4
            //- 0 0 1
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            cpu.set_c_flag();
            return 4;
        });
        add_op(0x38, +[](Cpu &cpu) -> clock_t {
            //JR C,r8
            //2  12/8
            //- - - -
            int8_t imm = cpu.fetch();
            if(cpu.c_flag_value()) {
                cpu.m_pc += imm;
                return 12;
            } else {
                return 8;
            }
        });
        add_op(0x39, +[](Cpu &cpu) -> clock_t {
            //ADD HL,SP
            //1  8
            //- 0 H C
            cpu.m_hl = add_nn(cpu, cpu.m_hl, cpu.m_sp);
            return 8;
        });
        add_op(0x3a, +[](Cpu &cpu) -> clock_t {
            //LD A,(HL-)
            //1  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_hl--;
            return 8;
        });
        add_op(0x3b, +[](Cpu &cpu) -> clock_t {
            //DEC SP
            //1  8
            //- - - -
            cpu.m_sp--;
            return 8;
        });
        add_op(0x3c, +[](Cpu &cpu) -> clock_t {
            //INC A
            //1  4
            //Z 0 H -
            cpu.m_a = inc_n(cpu, cpu.m_a);
            return 4;
        });
        add_op(0x3d, +[](Cpu &cpu) -> clock_t {
            //DEC A
            //1  4
            //Z 1 H -
            cpu.m_a = dec_n(cpu, cpu.m_a);
            return 4;
        });
        add_op(0x3e, +[](Cpu &cpu) -> clock_t {
            //LD A,d8
            //2  8
            //- - - -
            cpu.m_a = cpu.fetch();
            return 8;
        });
        add_op(0x3f, +[](Cpu &cpu) -> clock_t {
            //CCF
            //1  4
            //- 0 0 C
            if(cpu.c_flag_value()) {
                cpu.clear_c_flag();
            } else {
                cpu.set_c_flag();
            }
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            return 4;
        });
    }
}