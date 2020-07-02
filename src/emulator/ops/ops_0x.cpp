#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {

    void Cpu::add_0x() {
        add_op(0x00, +[](Cpu &cpu) -> clock_t {
            //NOP
            //1  4
            //- - - -
            return 4;
        });
        add_op(0x01, +[](Cpu &cpu) -> clock_t {
            //LD BC,d16
            //3  12
            //- - - -
            auto imm = cpu.wfetch();
            cpu.m_bc = imm;
            return 12;
        });
        add_op(0x02, +[](Cpu &cpu) -> clock_t {
            //LD (BC),A
            //1  8
            //- - - -
            cpu.m_mem.write_byte(cpu.m_bc, cpu.m_a);
            return 8;
        });
        add_op(0x03, +[](Cpu &cpu) -> clock_t {
            //INC BC
            //1  8
            //- - - -
            cpu.m_bc++;
            return 8;
        });
        add_op(0x04, +[](Cpu &cpu) -> clock_t {
            //INC B
            //1  4
            //Z 0 H -
            cpu.m_b = inc_n(cpu, cpu.m_b);
            return 4;
        });
        add_op(0x05, +[](Cpu &cpu) -> clock_t {
            //DEC B
            //1  4
            //Z 1 H -
            cpu.m_b = dec_n(cpu, cpu.m_b);
            return 4;
        });
        add_op(0x06, +[](Cpu &cpu) -> clock_t {
            //LD B,d8
            //2  8
            //- - - -
            auto imm = cpu.fetch();
            cpu.m_b = imm;
            return 8;
        });
        add_op(0x07, +[](Cpu &cpu) -> clock_t {
            //RLCA
            //1  4
            //0 0 0 C
            register_8_t  carry = (cpu.m_a & 0x80u) >> 7u;
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            cpu.update_c_flag(carry > 0 );
            register_8_t result = (cpu.m_a << 1u) + carry;
            cpu.m_a = result;
            return 4;
        });
        add_op(0x08, +[](Cpu &cpu) -> clock_t {
            //LD (a16),SP
            //3  20
            //- - - -
            auto imm = cpu.wfetch();
            cpu.m_mem.write_word(imm, cpu.m_sp);
            return 20;
        });
        add_op(0x09, +[](Cpu &cpu) -> clock_t {
            //ADD HL,BC
            //1  8
            //- 0 H C
            cpu.m_hl = add_nn(cpu, cpu.m_hl, cpu.m_bc);
            return 8;
        });
        add_op(0x0a, +[](Cpu &cpu) -> clock_t {
            //LD A,(BC)
            //1  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(cpu.m_bc);
            return 8;
        });
        add_op(0x0b, +[](Cpu &cpu) -> clock_t {
            //DEC BC
            //1  8
            //- - - -
            cpu.m_bc--;
            return 8;
        });
        add_op(0x0c, +[](Cpu &cpu) -> clock_t {
            //INC C
            //1  4
            //Z 0 H -
            cpu.m_c = inc_n(cpu, cpu.m_c);
            return 4;
        });
        add_op(0x0d, +[](Cpu &cpu) -> clock_t {
            //DEC C
            //1  4
            //Z 1 H -
            cpu.m_c = dec_n(cpu, cpu.m_c);
            return 4;
        });
        add_op(0x0e, +[](Cpu &cpu) -> clock_t {
            //LD C,d8
            //2  8
            //- - - -
            cpu.m_c = cpu.fetch();
            return 8;
        });
        add_op(0x0f, +[](Cpu &cpu) -> clock_t {
            //RRCA
            //1  4
            //0 0 0 C
            register_8_t carry = cpu.m_a & 0x01u;
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.clear_h_flag();
            cpu.update_c_flag(carry > 0);
            cpu.m_a = (cpu.m_a >> 1u) + (carry << 7u);
            return 4;
        });
    }
}