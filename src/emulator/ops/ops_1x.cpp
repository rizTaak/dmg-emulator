#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_1x() {
        add_op(0x10, +[](Cpu &cpu) -> clock_t {
            //STOP 0
            //2  4
            //- - - -
            cpu.halt();
            return 4;
        });
        add_op(0x11, +[](Cpu &cpu) -> clock_t {
            //LD DE,d16
            //3  12
            //- - - -
            register_16_t imm = cpu.wfetch();
            cpu.m_de = imm;
            return 12;
        });
        add_op(0x12, +[](Cpu &cpu) -> clock_t {
            //LD (DE),A
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_de, cpu.m_a);
            return 8;
        });
        add_op(0x13, +[](Cpu &cpu) -> clock_t {
            //INC DE
            //1  8
            //- - - -
            cpu.m_de++;
            return 8;
        });
        add_op(0x14, +[](Cpu &cpu) -> clock_t {
            //INC D
            //1  4
            //Z 0 H -
            cpu.m_d = inc_n(cpu, cpu.m_d);
            return 4;
        });
        add_op(0x15, +[](Cpu &cpu) -> clock_t {
            //DEC D
            //1  4
            //Z 1 H -
            cpu.m_d = dec_n(cpu, cpu.m_d);
            return 4;
        });
        add_op(0x16, +[](Cpu &cpu) -> clock_t {
            //LD D,d8
            //2  8
            //- - - -
            cpu.m_d = cpu.fetch();
            return 8;
        });
        add_op(0x17, +[](Cpu &cpu) -> clock_t {
            //RLA
            //1  4
            //0 0 0 C
            register_8_t  carry = cpu.c_flag_value() ? 1 : 0;
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            cpu.update_c_flag((cpu.m_a & 0x80u) > 0 );
            register_8_t result = (cpu.m_a << 1u) + carry;
            cpu.m_a = result;
            return 4;
        });
        add_op(0x18, +[](Cpu &cpu) -> clock_t {
            //JR r8
            //2  12
            //- - - -
            auto imm = static_cast<int8_t>(cpu.fetch());
            cpu.m_pc += imm;
            return 12;
        });
        add_op(0x19, +[](Cpu &cpu) -> clock_t {
            //ADD HL,DE
            //1  8
            //- 0 H C
            cpu.m_hl = add_nn(cpu, cpu.m_hl, cpu.m_de);
            return 8;
        });
        add_op(0x1a, +[](Cpu &cpu) -> clock_t {
            //LD A,(DE)
            //1  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(cpu.m_de);
            return 8;
        });
        add_op(0x1b, +[](Cpu &cpu) -> clock_t {
            //DEC DE
            //1  8
            //- - - -
            cpu.m_de--;
            return 8;
        });
        add_op(0x1c, +[](Cpu &cpu) -> clock_t {
            //INC E
            //1  4
            //Z 0 H -
            cpu.m_e = inc_n(cpu, cpu.m_e);
            return 4;
        });
        add_op(0x1d, +[](Cpu &cpu) -> clock_t {
            //DEC E
            //1  4
            //Z 1 H -
            cpu.m_e = dec_n(cpu, cpu.m_e);
            return 4;
        });
        add_op(0x1e, +[](Cpu &cpu) -> clock_t {
            //LD E,d8
            //2  8
            //- - - -
            auto imm = cpu.fetch();
            cpu.m_e = imm;
            return 8;
        });
        add_op(0x1f, +[](Cpu &cpu) -> clock_t {
            //RRA
            //1  4
            //0 0 0 C
            register_8_t last = cpu.m_a & 0x01u;
            register_8_t carry = cpu.c_flag_value() ? 0x80 : 0x00;
            register_8_t result = (cpu.m_a >> 1u) + carry;
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            cpu.update_c_flag(last > 0 );
            cpu.m_a = result;
            return 4;
        });
    }
}