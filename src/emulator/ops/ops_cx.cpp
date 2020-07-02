#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_cx() {
        add_op(0xc0, +[](Cpu &cpu) -> clock_t {
            //RET NZ
            //1  20/8
            //- - - -
            if (cpu.z_flag_value()) {
                return 8;
            } else {
                auto imm = cpu.wpop();
                cpu.m_pc = imm;
                return 20;
            }
        });
        add_op(0xc1, +[](Cpu &cpu) -> clock_t {
            //POP BC
            //1  12
            //- - - -
            register_16_t imm = cpu.wpop();
            cpu.m_bc = imm;
            return 12;
        });
        add_op(0xc2, +[](Cpu &cpu) -> clock_t {
            //JP NZ,a16
            //3  16/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.z_flag_value()) {
                return 12;
            } else {
                cpu.m_pc = imm;
                return 16;
            }
        });
        add_op(0xc3, +[](Cpu &cpu) -> clock_t {
            //JP a16
            //3  16
            //- - - -
            auto imm = cpu.wfetch();
            cpu.m_pc = imm;
            return 16;
        });
        add_op(0xc4, +[](Cpu &cpu) -> clock_t {
            //CALL NZ,a16
            //3  24/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.z_flag_value()) {
                return 12;
            } else {
                cpu.wpush(cpu.m_pc);
                cpu.m_pc = imm;
                return 24;
            }
        });
        add_op(0xc5, +[](Cpu &cpu) -> clock_t {
            //PUSH BC
            //1  16
            //- - - -
            cpu.wpush(cpu.m_bc);
            return 16;
        });
        add_op(0xc6, +[](Cpu &cpu) -> clock_t {
            //ADD A,d8
            //2  8
            //Z 0 H C
            auto imm = cpu.fetch();
            cpu.m_a = add_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xc7, +[](Cpu &cpu) -> clock_t {
            //RST 00H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0000u;
            return 16;
        });
        add_op(0xc8, +[](Cpu &cpu) -> clock_t {
            //RET Z
            //1  20/8
            //- - - -
            if(cpu.z_flag_value()) {
                cpu.m_pc = cpu.wpop();
                return 20;
            } else {
                return 8;
            }
        });
        add_op(0xc9, +[](Cpu &cpu) -> clock_t {
            //RET
            //1  16
            //- - - -
            auto imm = cpu.wpop();
            cpu.m_pc = imm;
            return 16;
        });
        add_op(0xca, +[](Cpu &cpu) -> clock_t {
            //JP Z,a16
            //3  16/12
            //- - - -
            auto imm = cpu.wfetch();
            if(cpu.z_flag_value()) {
                cpu.m_pc = imm;
                return 16;
            } else {
                return 12;
            }
        });
        add_op(0xcb, +[](Cpu &cpu) -> clock_t {
            //PREFIX CB
            //1  4
            //- - - -
            throw std::runtime_error("0xcb should not be processed as instruction");
            return 4;
        });
        add_op(0xcc, +[](Cpu &cpu) -> clock_t {
            //CALL Z,a16
            //3  24/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.z_flag_value()) {
                cpu.wpush(cpu.m_pc);
                cpu.m_pc = imm;
                return 24;
            } else {
                return 12;
            }
        });
        add_op(0xcd, +[](Cpu &cpu) -> clock_t {
            //CALL a16
            //3  24
            //- - - -
            auto imm = cpu.wfetch();
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = imm;
            return 24;
        });
        add_op(0xce, +[](Cpu &cpu) -> clock_t {
            //ADC A,d8
            //2  8
            //Z 0 H C
            auto imm = cpu.fetch();
            cpu.m_a = adc_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xcf, +[](Cpu &cpu) -> clock_t {
            //RST 08H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0008u;
            return 16;
        });
    }
}