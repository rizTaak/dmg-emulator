#ifndef EMULATOR_JOYPAD_H
#define EMULATOR_JOYPAD_H

#include <emulator/common.h>

namespace emulator {
    class Cpu;

    class JoyPad {
    public:
        static constexpr register_8_t btn_flag = 0b0010'0000;
        static constexpr register_8_t dir_flag = 0b0001'0000;

        static constexpr register_8_t dir_down = 0b0000'1000;
        static constexpr register_8_t dir_up = 0b0000'0100;
        static constexpr register_8_t dir_left = 0b0000'0010;
        static constexpr register_8_t dir_right = 0b0000'0001;

        static constexpr register_8_t btn_start = 0b0000'1000;
        static constexpr register_8_t btn_select = 0b0000'0100;
        static constexpr register_8_t btn_b = 0b0000'0010;
        static constexpr register_8_t btn_a = 0b0000'0001;
    private:
        register_8_t m_dir;
        register_8_t m_btn;
        Cpu &m_cpu;

        void press(register_8_t &val, register_8_t key);

        static inline void release(register_8_t &val, register_8_t key) {
            val = val | key;
        }

    public:
        JoyPad(const JoyPad &other) = delete;
        JoyPad(const JoyPad &&other) = delete;
        JoyPad &operator=(const JoyPad &) = delete;
        JoyPad &operator=(JoyPad &&) = delete;

        explicit JoyPad(Cpu &cpu):
                m_cpu{cpu},
                m_dir{0x0f},
                m_btn{0x0f} {
        }

        [[nodiscard]] inline register_8_t dir() const {
            return m_dir;
        }

        [[nodiscard]]  inline register_8_t btn() const {
            return m_btn;
        }

        void press_up() {
            press(m_dir, dir_up);
        }

        void press_down() {
            press(m_dir, dir_down);
        }

        void press_left() {
            press(m_dir, dir_left);
        }

        void press_right() {
            press(m_dir, dir_right);
        }

        void press_select() {
            press(m_btn, btn_select);
        }

        void press_start() {
            press(m_btn, btn_start);
        }

        void press_a() {
            press(m_btn, btn_a);
        }

        void press_b() {
            press(m_btn, btn_b);
        }

        void release_up() {
            release(m_dir, dir_up);
        }

        void release_down() {
            release(m_dir, dir_down);
        }

        void release_left() {
            release(m_dir, dir_left);
        }

        void release_right() {
            release(m_dir, dir_right);
        }

        void release_select() {
            release(m_btn, btn_select);
        }

        void release_start() {
            release(m_btn, btn_start);
        }

        void release_a() {
            release(m_btn, btn_a);
        }

        void release_b() {
            release(m_btn, btn_b);
        }

    };
}

#endif