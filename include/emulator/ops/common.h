#ifndef EMULATOR_OPS_COMMON_H
#define EMULATOR_OPS_COMMON_H

#include <emulator/common.h>
#include <emulator/cpu.h>

namespace emulator {
    register_8_t dec_n(Cpu &cpu, register_8_t value);
    register_8_t  inc_n(Cpu &cpu, register_8_t value);

    register_8_t sub_n(Cpu &cpu, register_8_t from, register_8_t value);
    register_8_t sbc_n(Cpu &cpu, register_8_t from, register_8_t value);

    register_8_t add_n(Cpu &cpu, register_8_t from, register_8_t value);
    register_8_t adc_n(Cpu &cpu, register_8_t from, register_8_t value);

    register_8_t  and_n(Cpu &cpu, register_8_t from, register_8_t value);
    register_8_t  xor_n(Cpu &cpu, register_8_t from, register_8_t value);
    register_8_t  or_n(Cpu &cpu, register_8_t from, register_8_t value);

    register_8_t rl_n(Cpu &cpu, register_8_t value);
    register_8_t rr_n(Cpu &cpu, register_8_t value);

    register_8_t rlc_n(Cpu &cpu, register_8_t value);
    register_8_t rrc_n(Cpu &cpu, register_8_t value);

    register_8_t sla_n(Cpu &cpu, register_8_t value);
    register_8_t sra_n(Cpu &cpu, register_8_t value);

    register_8_t swap_n(Cpu &cpu, register_8_t value);

    register_8_t srl_n(Cpu &cpu, register_8_t value);

    register_16_t add_nn(Cpu &cpu, register_16_t to, register_16_t value);

    void bit(Cpu &cpu, register_8_t bit, register_8_t value);
    void bit_0(Cpu &cpu, register_8_t value);
    void bit_1(Cpu &cpu, register_8_t value);
    void bit_2(Cpu &cpu, register_8_t value);
    void bit_3(Cpu &cpu, register_8_t value);
    void bit_4(Cpu &cpu, register_8_t value);
    void bit_5(Cpu &cpu, register_8_t value);
    void bit_6(Cpu &cpu, register_8_t value);
    void bit_7(Cpu &cpu, register_8_t value);

    register_8_t res(Cpu &cpu, register_8_t bit, register_8_t value);
    register_8_t res_0(Cpu &cpu, register_8_t value);
    register_8_t res_1(Cpu &cpu, register_8_t value);
    register_8_t res_2(Cpu &cpu, register_8_t value);
    register_8_t res_3(Cpu &cpu, register_8_t value);
    register_8_t res_4(Cpu &cpu, register_8_t value);
    register_8_t res_5(Cpu &cpu, register_8_t value);
    register_8_t res_6(Cpu &cpu, register_8_t value);
    register_8_t res_7(Cpu &cpu, register_8_t value);

    register_8_t set(Cpu &cpu, register_8_t bit, register_8_t value);
    register_8_t set_0(Cpu &cpu, register_8_t value);
    register_8_t set_1(Cpu &cpu, register_8_t value);
    register_8_t set_2(Cpu &cpu, register_8_t value);
    register_8_t set_3(Cpu &cpu, register_8_t value);
    register_8_t set_4(Cpu &cpu, register_8_t value);
    register_8_t set_5(Cpu &cpu, register_8_t value);
    register_8_t set_6(Cpu &cpu, register_8_t value);
    register_8_t set_7(Cpu &cpu, register_8_t value);
}

#endif