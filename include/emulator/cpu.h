#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include <memory>
#include <unordered_map>
#include <deque>
#include <logging/logging.h>
#include <emulator/common.h>
#include <emulator/op.h>
#include <emulator/memory.h>
#include <emulator/gpu.h>
#include <emulator/mem_registers.h>

namespace emulator {
    class Gpu;

    class Cpu {
        spdlog::logger log;
        std::deque<std::string> m_previous_states;
        static const register_8_t z_flag = 0b1000'0000;
        static const register_8_t n_flag = 0b0100'0000;
        static const register_8_t h_flag = 0b0010'0000;
        static const register_8_t c_flag = 0b0001'0000;
        static const register_8_t f3_flag = 0b0000'1000;
        static const register_8_t f2_flag = 0b0000'0100;
        static const register_8_t f1_flag = 0b0000'0010;
        static const register_8_t f0_flag = 0b0000'0001;

        static const register_8_t vblank_interrupt  = 0b0000'0001;
        static const register_8_t lcdstat_interrupt = 0b0000'0010;
        static const register_8_t timer_interrupt   = 0b0000'0100;
        static const register_8_t serial_interrupt  = 0b0000'1000;
        static const register_8_t joypad_interrupt  = 0b0001'0000;



        union {
            register_16_t m_af;
            struct {
                register_8_t m_f{};
                register_8_t m_a{};
            };
        };
        union {
            register_16_t m_bc;
            struct {
                register_8_t m_c{};
                register_8_t m_b{};
            };
        };
        union {
            register_16_t m_de;
            struct {
                register_8_t m_e{};
                register_8_t m_d{};
            };
        };
        union {
            register_16_t m_hl;
            struct {
                register_8_t m_l{};
                register_8_t m_h{};
            };
        };
        register_16_t m_pc{}, m_sp{};
        clock_t m_last{0}, m_total{0};
        bool m_halted{false};
        bool m_stopped{false};
        bool m_haltbug{false};
        register_16_t m_haltbug_pc{};

        std::unordered_map<opcode_t, OpCode> m_ops;
        std::unordered_map<opcode_t, OpCode> m_bc_ops;
        size_t m_ins_count {};
        Memory &m_mem;
        Gpu *m_gpu;

        bool m_master_interrupt {false};
        //register_8_t m_mi_disable_pending {};
        register_8_t m_mi_enable_pending {};


        register_8_t &m_interrupt_enable_register;
        register_8_t &m_interrupt_req_register;
    public:
        clock_t last_ticks() const noexcept {
            return m_last;
        }

        clock_t total_ticks() const noexcept {
            return m_total;
        }

        explicit Cpu(Memory &mem);

        void connect_gpu(Gpu *gpu) {
            m_gpu = gpu;
        }

        Cpu(Cpu &other) = delete;

        Cpu(Cpu &&other) = delete;

        void reset();

        void fetch_decode_execute();

        inline void halt() {
            m_halted = true;
        }

        inline void unhalt() {
            m_halted = false;
        }

        inline bool halted() {
            return m_halted;
        }

        inline void enable_halt_bug() {
            m_haltbug = true;
            m_haltbug_pc = m_pc;
        }

        inline void clear_halt_bug() {
            m_haltbug = false;
            m_haltbug_pc = {};
        }

        void stop();

        void skip_boot();

        void add_op(opcode_t code, clock_t (*executor)(Cpu &z80)) {
            m_ops.emplace(code, OpCode(*this, code, executor));
        }

        void add_bc_op(opcode_t code, clock_t (*executor)(Cpu &z80)) {
            m_bc_ops.emplace(code, OpCode(*this, code, executor));
        }

        inline void set_z_flag() noexcept {
            set_flag(m_f, z_flag);
        }

        inline void update_z_flag(register_8_t value) noexcept {
            if (0 == value) set_z_flag(); else clear_z_flag();
        }

        inline void update_n_flag(bool value) noexcept {
            if (value) set_n_flag(); else clear_n_flag();
        }

        inline void update_c_flag(bool value) noexcept {
            if (value) set_c_flag(); else clear_c_flag();
        }

        inline void update_h_flag(register_8_t before, register_8_t after) noexcept {
            auto value = ((before & 0xf0u) != (after & 0xf0u));
            if (value) set_h_flag(); else clear_h_flag();
        }

        inline void update_h_flag(bool value) noexcept {
            if (value) set_h_flag(); else clear_h_flag();
        }

        inline void clear_z_flag() noexcept {
            clear_flag(m_f, z_flag);
        }

        inline void set_n_flag() noexcept {
            set_flag(m_f, n_flag);
        }

        inline void clear_n_flag() noexcept {
            clear_flag(m_f, n_flag);
        }

        inline void set_h_flag() noexcept {
            set_flag(m_f, h_flag);
        }

        inline void clear_h_flag() noexcept {
            clear_flag(m_f, h_flag);
        }

        inline void set_c_flag() noexcept {
            set_flag(m_f, c_flag);
        }

        inline void clear_c_flag() noexcept {
            clear_flag(m_f, c_flag);
        }

        inline void clear_all_flags() noexcept {
            m_f = 0x00;
        }

        inline bool z_flag_zero() noexcept {
            auto result = get_flag(m_f, z_flag);
            return result != 0;
        }

        inline bool c_flag_value() noexcept {
            auto result = get_flag(m_f, c_flag);
            return result != 0;
        }

        inline bool z_flag_value() noexcept {
            auto result = get_flag(m_f, z_flag);
            return result != 0;
        }

        inline bool n_flag_value() noexcept {
            auto result = get_flag(m_f, n_flag);
            return result != 0;
        }

        inline bool h_flag_value() noexcept {
            auto result = get_flag(m_f, h_flag);
            return result != 0;
        }

        // VBLANK
        inline bool vblank_req() noexcept {
            return get_flag_r(m_interrupt_req_register, vblank_interrupt);
        }

        inline bool vblank_enabled() noexcept {
            return get_flag_r(m_interrupt_enable_register, vblank_interrupt);
        }

        inline void vblank_set_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                set_flag_r(m_mem.read_byte(mem_registers::interrupt_req_register), vblank_interrupt));
        }

        inline void vblank_clear_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_req_register), vblank_interrupt));
        }

        inline void vblank_enable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             set_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), vblank_interrupt));
        }

        inline void vblank_disable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), vblank_interrupt));
        }

        // TIMER
        inline bool timer_req() noexcept {
            return get_flag_r(m_interrupt_req_register, timer_interrupt);
        }

        inline bool timer_enabled() noexcept {
            return get_flag_r(m_interrupt_enable_register, timer_interrupt);
        }

        inline void timer_set_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             set_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_req_register), timer_interrupt));
        }

        inline void timer_clear_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_req_register), timer_interrupt));
        }

        inline void timer_enable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             set_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), timer_interrupt));
        }

        inline void timer_disable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), timer_interrupt));
        }


        // JOYPAD
        inline bool joy_pad_req() noexcept {
            return get_flag_r(m_interrupt_req_register, joypad_interrupt);
        }

        inline bool joy_pad_enabled() noexcept {
            return get_flag_r(m_interrupt_enable_register, joypad_interrupt);
        }

        inline void joy_pad_set_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             set_flag_r(m_mem.read_byte(mem_registers::interrupt_req_register), joypad_interrupt));
        }

        inline void joy_pad_clear_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_req_register), joypad_interrupt));
        }

        inline void joy_pad_enable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             set_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), joypad_interrupt));
        }

        inline void joy_pad_disable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), joypad_interrupt));
        }


        // LCDC
        inline bool lcdc_req() noexcept {
            return get_flag_r(m_interrupt_req_register, lcdstat_interrupt);
        }

        inline bool lcdc_enabled() noexcept {
            return get_flag_r(m_interrupt_enable_register, lcdstat_interrupt);
        }

        inline void lcdc_set_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             set_flag_r(m_mem.read_byte(mem_registers::interrupt_req_register), lcdstat_interrupt));
        }

        inline void lcdc_clear_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_req_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_req_register), lcdstat_interrupt));
        }

        inline void lcdc_enable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             set_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), lcdstat_interrupt));
        }

        inline void lcdc_disable_interrupt() noexcept {
            m_mem.write_byte(mem_registers::interrupt_enable_register,
                             clear_flag_r(
                                     m_mem.read_byte(mem_registers::interrupt_enable_register), lcdstat_interrupt));
        }

        std::string get_pretty_state();
        void add_state(std::string &state);
        void log_history();
        inline bool any_interrupts_pending() const {
            return (m_interrupt_req_register & m_interrupt_enable_register) != 0x00;
        }

        inline bool should_unhalt() const {
            return m_interrupt_req_register > 0x00;
        }

        void init_for_catridge();

        register_16_t current_pc() {
            return m_pc;
        }

    private:
        inline static void set_flag(register_8_t &r, register_8_t mask) noexcept {
            r = r | mask;
        }

        inline static void clear_flag(register_8_t &r, register_8_t mask) noexcept {
            r = r & ~mask;
        }

        inline static register_8_t get_flag(register_8_t &r, register_8_t mask) noexcept {
            return r & mask;
        }

        inline static register_8_t set_flag_r(register_8_t r, register_8_t mask) noexcept {
            return r | mask;
        }

        inline static register_8_t clear_flag_r(register_8_t r, register_8_t mask) noexcept {
            return r & ~mask;
        }

        inline static bool get_flag_r(register_8_t r, register_8_t mask) noexcept {
            return r & mask;
        }

        register_8_t fetch();
        register_16_t wfetch();
        register_16_t peek_pc();

        void add_0x();
        void add_1x();
        void add_2x();
        void add_3x();
        void add_4x();
        void add_5x();
        void add_6x();
        void add_7x();
        void add_8x();
        void add_9x();
        void add_ax();
        void add_bx();
        void add_cx();
        void add_dx();
        void add_ex();
        void add_fx();

        void add_cb0x();
        void add_cb1x();
        void add_cb2x();
        void add_cb3x();
        void add_cb4x();
        void add_cb5x();
        void add_cb6x();
        void add_cb7x();
        void add_cb8x();
        void add_cb9x();
        void add_cbax();
        void add_cbbx();
        void add_cbcx();
        void add_cbdx();
        void add_cbex();
        void add_cbfx();

        inline static register_8_t get_bit(register_8_t bit, register_8_t val) noexcept {
            return (1u << bit) & val;
        }

        void wpush(register_16_t value);
        register_16_t wpop();

        void enable_interrupt();
        void disable_interrupt();
        void handle_mi();
        clock_t handle_interrupt();
        clock_t handle_vblank_interrupt();
        clock_t handle_lcdc_interrupt();
        clock_t handel_timer_interrupt();
        clock_t handel_joy_pad_interrupt();
        void handle_halt_bug();
    };
}

#endif