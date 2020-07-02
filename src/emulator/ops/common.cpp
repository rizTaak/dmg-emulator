#include <emulator/ops/common.h>

namespace emulator {
    register_8_t dec_n(Cpu &cpu, register_8_t from) {
        register_8_t value = 1;
        register_8_t result = from - value;
        cpu.update_h_flag((from & 0x0fu) < (value & 0x0fu));
        cpu.update_z_flag(result);
        cpu.set_n_flag();
        return result;
    }

    register_8_t  inc_n(Cpu &cpu, register_8_t from){
        register_8_t value = 1;
        auto result = from + value;
        cpu.update_z_flag(result);
        cpu.clear_n_flag();
        cpu.update_h_flag((from & 0x0fu) + (value & 0x0fu) > 0x0fu);
        return result;
    }

    register_8_t sub_n(Cpu &cpu, register_8_t from, register_8_t value) {
        register_8_t result = from-value;
        cpu.update_z_flag(result);
        cpu.set_n_flag();
        cpu.update_h_flag((from & 0x0fu) < (value & 0x0fu));
        cpu.update_c_flag(from < value);
        return result;
    }

    register_8_t add_n(Cpu &cpu, register_8_t from, register_8_t value) {
        register_16_t intmd = from+value;
        auto result = static_cast<register_8_t>(intmd & 0x00ffu);
        cpu.update_h_flag((from & 0x0fu) + (value & 0x0fu) > 0x0fu);
        cpu.update_z_flag(result);
        cpu.update_c_flag((intmd & 0xff00u) > 0);
        cpu.clear_n_flag();
        return result;
    }

    register_8_t adc_n(Cpu &cpu, register_8_t from, register_8_t value) {
        register_8_t carry = cpu.c_flag_value();
        register_16_t result_full = from + value + carry;
        auto result = static_cast<register_8_t>(result_full);
        cpu.update_z_flag(result);
        cpu.clear_n_flag();
        cpu.update_h_flag(((from & 0xfu) + (value & 0xfu) + carry) > 0xf);
        cpu.update_c_flag(result_full > 0xff);
        return result;
    }

    register_8_t sbc_n(Cpu &cpu, register_8_t from, register_8_t value) {
        register_8_t carry = cpu.c_flag_value();
        int16_t result_full = from - value - carry;
        auto result = static_cast<register_8_t>(result_full);
        cpu.update_z_flag(result);
        cpu.set_n_flag();
        cpu.update_c_flag(result_full < 0);
        //todo: why 0xfu is failing?
        cpu.update_h_flag(((from & 0xf) - (value & 0xf) - carry) < 0);
        return result;
    }

    register_16_t add_nn(Cpu &cpu, register_16_t to, register_16_t value) {
        register_32_t intmd = to+value;
        auto result = static_cast<register_16_t>(intmd & 0x0000ffffu);
        cpu.update_h_flag((to & 0x0fffu) + (value & 0x0fffu) > 0x0fffu);
        cpu.update_c_flag((intmd & 0xffff0000u) > 0);
        cpu.clear_n_flag();
        return result;
    }

    register_8_t  and_n(Cpu &cpu, register_8_t from, register_8_t value) {
        auto result = from & value;
        cpu.update_z_flag(result);
        cpu.clear_n_flag();
        cpu.set_h_flag();
        cpu.clear_c_flag();
        return result;
    }

    register_8_t  xor_n(Cpu &cpu, register_8_t from, register_8_t value) {
        auto result = from ^ value;
        cpu.update_z_flag(result);
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.clear_c_flag();
        return result;
    }

    register_8_t  or_n(Cpu &cpu, register_8_t from, register_8_t value) {
        auto result = from | value;
        cpu.update_z_flag(result);
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.clear_c_flag();
        return result;
    }

    register_8_t rl_n(Cpu &cpu, register_8_t value) {
        register_8_t  carry = cpu.c_flag_value() ? 1 : 0;
        cpu.update_c_flag((value & 0x80u) > 0 );
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        register_8_t result = (value << 1u) + carry;
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t rr_n(Cpu &cpu, register_8_t value) {
        register_8_t  carry = cpu.c_flag_value() ? 1 : 0;
        register_8_t  last = value & 0x01u;
        cpu.update_c_flag(last > 0 );
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        register_8_t result = (value >> 1u) + (carry << 7u);
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t rlc_n(Cpu &cpu, register_8_t value) {
        register_8_t  carry = (value & 0x80u) >> 7u;
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.update_c_flag(carry > 0 );
        register_8_t result = (value << 1u) + carry;
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t rrc_n(Cpu &cpu, register_8_t value) {
        register_8_t  carry = (value & 0x01u);
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.update_c_flag(carry > 0 );
        register_8_t result = (value >> 1u) + (carry << 7u);
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t sla_n(Cpu &cpu, register_8_t value) {
        register_8_t  first = (value & 0x80u);
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.update_c_flag(first > 0 );
        register_8_t result = value << 1u;
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t sra_n(Cpu &cpu, register_8_t value) {
        register_8_t  first = (value & 0x80u);
        cpu.clear_n_flag();
        cpu.clear_h_flag();
        cpu.update_c_flag((value & 0x01u) > 0);
        register_8_t result = first + (value >> 1u);
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t swap_n(Cpu &cpu, register_8_t value) {
        auto result = (value << 0x04u) | (value >> 0x04u);
        cpu.clear_c_flag();
        cpu.clear_h_flag();
        cpu.clear_n_flag();
        cpu.update_z_flag(result);
        return result;
    }

    register_8_t srl_n(Cpu &cpu, register_8_t value) {
        auto last = value & 0x01u;
        auto result = value >> 0x01u;
        cpu.update_c_flag(last > 0);
        cpu.clear_h_flag();
        cpu.clear_n_flag();
        cpu.update_z_flag(result);
        return result;
    }

    void bit(Cpu &cpu, register_8_t bit, register_8_t value) {
        if(bit & value) {
            cpu.clear_z_flag();
        } else {
            cpu.set_z_flag();
        }
        cpu.clear_n_flag();
        cpu.set_h_flag();
    }
    void bit_0(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x0u, value);
    }
    void bit_1(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x01u, value);
    }
    void bit_2(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x02u, value);
    }
    void bit_3(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x03u, value);
    }
    void bit_4(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x04u, value);
    }
    void bit_5(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x05u, value);
    }
    void bit_6(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x06u, value);
    }
    void bit_7(Cpu &cpu, register_8_t value) {
        bit(cpu, 0x01u << 0x07u, value);
    }


    register_8_t res(Cpu &cpu, register_8_t bit, register_8_t value) {
        return (~bit) & value;
    }
    register_8_t res_0(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x00u, value);
    }
    register_8_t res_1(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x01u, value);
    }
    register_8_t res_2(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x02u, value);
    }
    register_8_t res_3(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x03u, value);
    }
    register_8_t res_4(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x04u, value);
    }
    register_8_t res_5(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x05u, value);
    }
    register_8_t res_6(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x06u, value);
    }
    register_8_t res_7(Cpu &cpu, register_8_t value) {
        return res(cpu, 0x01u << 0x07u, value);
    }


    register_8_t set(Cpu &cpu, register_8_t bit, register_8_t value) {
        return bit | value;
    }
    register_8_t set_0(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x00u, value);
    }
    register_8_t set_1(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x01u, value);
    }
    register_8_t set_2(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x02u, value);
    }
    register_8_t set_3(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x03u, value);
    }
    register_8_t set_4(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x04u, value);
    }
    register_8_t set_5(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x05u, value);
    }
    register_8_t set_6(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x06u, value);
    }
    register_8_t set_7(Cpu &cpu, register_8_t value) {
        return set(cpu, 0x01u << 0x07u, value);
    }
}