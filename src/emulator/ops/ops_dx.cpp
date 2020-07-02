#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_dx() {
        add_op(0xd0, +[](Cpu &cpu) -> clock_t {
            //RET NC
            //1  20/8
            //- - - -
            if(cpu.c_flag_value()) {
                return 8;
            } else {
                auto imm = cpu.wpop();
                cpu.m_pc = imm;
                return 20;
            }
        });
        add_op(0xd1, +[](Cpu &cpu) -> clock_t {
            //POP DE
            //1  12
            //- - - -
            auto imm = cpu.wpop();
            cpu.m_de = imm;
            return 12;
        });
        add_op(0xd2, +[](Cpu &cpu) -> clock_t {
            //JP NC,a16
            //3  16/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.c_flag_value()) {
                return 12;
            } else {
                cpu.m_pc = imm;
                return 16;
            }
        });
        add_op(0xd3, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xd4, +[](Cpu &cpu) -> clock_t {
            //CALL NC,a16
            //3  24/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.c_flag_value()) {
                return 12;
            } else {
                cpu.wpush(cpu.m_pc);
                cpu.m_pc = imm;
                return 24;
            }
        });
        add_op(0xd5, +[](Cpu &cpu) -> clock_t {
            //PUSH DE
            //1  16
            //- - - -
            cpu.wpush(cpu.m_de);
            return 16;
        });
        add_op(0xd6, +[](Cpu &cpu) -> clock_t {
            //SUB d8
            //2  8
            //Z 1 H C
            auto imm = cpu.fetch();
            cpu.m_a = sub_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xd7, +[](Cpu &cpu) -> clock_t {
            //RST 10H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0010u;
            return 16;
        });
        add_op(0xd8, +[](Cpu &cpu) -> clock_t {
            //RET C
            //1  20/8
            //- - - -
            if (cpu.c_flag_value()) {
                auto imm = cpu.wpop();
                cpu.m_pc = imm;
                return 20;
            } else {
                return 8;
            }
        });
        add_op(0xd9, +[](Cpu &cpu) -> clock_t {
            //RETI
            //1  16
            //- - - -
            //cpu.log.info("reti");
            in_vblank = false;
            cpu.m_pc = cpu.wpop();
            cpu.enable_interrupt();
            return 16;
        });
        add_op(0xda, +[](Cpu &cpu) -> clock_t {
            //JP C,a16
            //3  16/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.c_flag_value()) {
                cpu.m_pc = imm;
                return 16;
            } else {
                return 12;
            }
        });
        add_op(0xdb, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xdc, +[](Cpu &cpu) -> clock_t {
            //CALL C,a16
            //3  24/12
            //- - - -
            auto imm = cpu.wfetch();
            if (cpu.c_flag_value()) {
                cpu.wpush(cpu.m_pc);
                cpu.m_pc = imm;
                return 24;
            } else {
                return 12;
            }
        });
        add_op(0xdd, +[](Cpu &cpu) -> clock_t {
            //
            throw std::runtime_error("nothing to see here");
            return 4;
        });
        add_op(0xde, +[](Cpu &cpu) -> clock_t {
            //SBC A,d8
            //2  8
            //Z 1 H C
            auto imm = cpu.fetch();
            cpu.m_a = sbc_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0xdf, +[](Cpu &cpu) -> clock_t {
            //RST 18H
            //1  16
            //- - - -
            cpu.wpush(cpu.m_pc);
            cpu.m_pc = 0x0018u;
            return 16;
        });
    }
}