#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_ex() {
        add_op(0xe0, +[](Cpu &cpu) -> clock_t {
            //LDH (a8),A
            //2  12
            //- - - -
            register_16_t imm = 0xff00+cpu.fetch();
            cpu.m_mem.write_byte(imm, cpu.m_a);
            if (imm == 0xff50) {
                cpu.init_for_catridge();
            }
            return 12;
        });
        add_op(0xe1, +[](Cpu &cpu) -> clock_t {
            //POP HL
            //1  12
            //- - - -
           cpu.m_hl = cpu.wpop();
            return 12;
        });
        add_op(0xe2, +[](Cpu &cpu) -> clock_t {
            //LD (C),A
            //2  8
            //- - - -
            cpu.m_mem.write_byte((0xFF00+cpu.m_c), cpu.m_a);
            return 8;
        });
        add_op(0xe3, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xe4, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xe5, +[](Cpu &cpu) -> clock_t {
            //PUSH HL
            //1  16
            //- - - -
            cpu.wpush(cpu.m_hl);
            return 16;
        });
        add_op(0xe6, +[](Cpu &cpu) -> clock_t {
            //AND d8
            //2  8
            //Z 0 1 0
            auto imm = cpu.fetch();
            cpu.m_a = and_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xe7, +[](Cpu &cpu) -> clock_t {
            //RST 20H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0020u;
            return 16;
        });
        add_op(0xe8, +[](Cpu &cpu) -> clock_t {
            //ADD SP,r8
            //2  16
            //0 0 H C
            register_16_t reg = cpu.m_sp;
            auto value = static_cast<int8_t>(cpu.fetch());
            int32_t result = static_cast<int32_t>(reg + value);
            cpu.clear_z_flag();
            cpu.clear_n_flag();
            cpu.update_h_flag(((reg ^ value ^ (result & 0xffff)) & 0x10) == 0x10);
            cpu.update_c_flag(((reg ^ value ^ (result & 0xffff)) & 0x100) == 0x100);
            cpu.m_sp = static_cast<register_16_t>(result);
            return 16;
        });
        add_op(0xe9, +[](Cpu &cpu) -> clock_t {
            //JP (HL)
            //1  4
            //- - - -
            cpu.m_pc = cpu.m_hl;
            return 4;
        });
        add_op(0xea, +[](Cpu &cpu) -> clock_t {
            //LD (a16),A
            //3  16
            //- - - -
            auto imm = cpu.wfetch();
            cpu.m_mem.write_byte(imm, cpu.m_a);
            return 16;
        });
        add_op(0xeb, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xec, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xed, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xee, +[](Cpu &cpu) -> clock_t {
            //XOR d8
            //2  8
            //Z 0 0 0
            auto imm = cpu.fetch();
            cpu.m_a = xor_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xef, +[](Cpu &cpu) -> clock_t {
            //RST 28H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0028u;
            return 16;
        });
    }
}