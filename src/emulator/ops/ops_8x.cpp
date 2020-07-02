#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/ops/common.h>

namespace emulator {
    void Cpu::add_8x() {
        add_op(0x80, +[](Cpu &cpu) -> clock_t {
            //ADD A,B
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0x81, +[](Cpu &cpu) -> clock_t {
            //ADD A,C
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0x82, +[](Cpu &cpu) -> clock_t {
            //ADD A,D
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0x83, +[](Cpu &cpu) -> clock_t {
            //ADD A,E
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0x84, +[](Cpu &cpu) -> clock_t {
            //ADD A,H
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0x85, +[](Cpu &cpu) -> clock_t {
            //ADD A,L
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0x86, +[](Cpu &cpu) -> clock_t {
            //ADD A,(HL)
            //1  8
            //Z 0 H C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = add_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0x87, +[](Cpu &cpu) -> clock_t {
            //ADD A,A
            //1  4
            //Z 0 H C
            cpu.m_a = add_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
        add_op(0x88, +[](Cpu &cpu) -> clock_t {
            //ADC A,B
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_b);
            return 4;
        });
        add_op(0x89, +[](Cpu &cpu) -> clock_t {
            //ADC A,C
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_c);
            return 4;
        });
        add_op(0x8a, +[](Cpu &cpu) -> clock_t {
            //ADC A,D
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_d);
            return 4;
        });
        add_op(0x8b, +[](Cpu &cpu) -> clock_t {
            //ADC A,E
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_e);
            return 4;
        });
        add_op(0x8c, +[](Cpu &cpu) -> clock_t {
            //ADC A,H
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_h);
            return 4;
        });
        add_op(0x8d, +[](Cpu &cpu) -> clock_t {
            //ADC A,L
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_l);
            return 4;
        });
        add_op(0x8e, +[](Cpu &cpu) -> clock_t {
            //ADC A,(HL)
            //1  8
            //Z 0 H C
            auto imm = cpu.m_mem.read_byte(cpu.m_hl);
            cpu.m_a = adc_n(cpu, cpu.m_a, imm);
            return 8;
        });
        add_op(0x8f, +[](Cpu &cpu) -> clock_t {
            //ADC A,A
            //1  4
            //Z 0 H C
            cpu.m_a = adc_n(cpu, cpu.m_a, cpu.m_a);
            return 4;
        });
    }
}