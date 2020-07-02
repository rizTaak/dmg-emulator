
#include <emulator/joypad.h>
#include <emulator/cpu.h>

namespace emulator {
    void JoyPad::press(register_8_t &val, register_8_t key) {
        val = val & (~key);
        m_cpu.joy_pad_set_interrupt();
    }
}