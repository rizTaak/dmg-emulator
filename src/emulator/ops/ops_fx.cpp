#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_fx() {
        add_op(0xf0, +[](Cpu &cpu) -> clock_t {
            //LDH A,(a8)
            //2  12
            //- - - -
            register_8_t imm = cpu.fetch();
            cpu.m_a = cpu.m_mem.read_byte((0xFF00+imm));
            return 12;
        });
        add_op(0xf1, +[](Cpu &cpu) -> clock_t {
            //POP AF
            //1  12
            //Z N H C
            cpu.m_af = cpu.wpop();
            cpu.m_f = cpu.m_f & 0xf0;
            return 12;
        });
        add_op(0xf2, +[](Cpu &cpu) -> clock_t {
            //LD A,(C)
            //2  8
            //- - - -
            cpu.m_a = cpu.m_mem.read_byte(0xff00u + cpu.m_c);
            return 8;
        });
        add_op(0xf3, +[](Cpu &cpu) -> clock_t {
            //DI
            //1  4
            //- - - -
            cpu.disable_interrupt();
            return 4;
        });
        add_op(0xf4, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xf5, +[](Cpu &cpu) -> clock_t {
            //PUSH AF
            //1  16
            //- - - -
            cpu.wpush(cpu.m_af);
            return 16;
        });
        add_op(0xf6, +[](Cpu &cpu) -> clock_t {
            //OR d8
            //2  8
            //Z 0 0 0
            auto imm = cpu.fetch();
            cpu.m_a = or_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xf7, +[](Cpu &cpu) -> clock_t {
            //RST 30H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0030u;
            return 16;
        });
        add_op(0xf8, +[](Cpu &cpu) -> clock_t {
            //LD HL,SP+r8
            //2  12
            //0 0 H C
            register_16_t reg = cpu.m_sp;
            auto value = static_cast<int8_t>(cpu.fetch());
            int32_t result = static_cast<int32_t>(reg + value);
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.update_h_flag(((reg ^ value ^ (result & 0xffff)) & 0x10) == 0x10);
            cpu.update_c_flag(((reg ^ value ^ (result & 0xffff)) & 0x100) == 0x100);
            cpu.m_hl = static_cast<register_16_t>(result);
            return 12;
        });
        add_op(0xf9, +[](Cpu &cpu) -> clock_t {
            //LD SP,HL
            //1  8
            //- - - -
            cpu.m_sp = cpu.m_hl;
            return 8;
        });
        add_op(0xfa, +[](Cpu &cpu) -> clock_t {
            //LD A,(a16)
            //3  16
            //- - - -
            auto imm = cpu.wfetch();
            cpu.m_a = cpu.m_mem.read_byte(imm);
            return 16;
        });
        add_op(0xfb, +[](Cpu &cpu) -> clock_t {
            //EI
            //1  4
            //- - - -
            cpu.enable_interrupt();
            return 4;
        });
        add_op(0xfc, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xfd, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xfe, +[](Cpu &cpu) -> clock_t {
            //CP d8
            //2  8
            //Z 1 H C
            auto imm = cpu.fetch();
            sub_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xff, +[](Cpu &cpu) -> clock_t {
            //RST 38H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0038u;
            return 16;
        });
    }
}