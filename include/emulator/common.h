#ifndef EMULATOR_COMMON_H
#define EMULATOR_COMMON_H

#include <cstdint>
#include <csignal>

namespace emulator {
    using register_8_t = uint8_t;
    using register_16_t = uint16_t;
    using register_32_t = uint32_t;
    using clock_t = uint64_t;
    using opcode_t = uint8_t;

    void debug_break();

    static bool in_vblank = false;

    enum cartridge_type : register_8_t {
        rom_only = 0x00,
        mbc1 = 0x01,
        mbc1_ram = 0x02,
        mbc1_ram_battery = 0x03,
        mbc5 = 0x19,
        mbc5_ram = 0x1a,
        mbc5_ram_battery = 0x1b,
        mbc5_rumble = 0x1c,
        mbc5_rumble_ram = 0x1d,
        mbc5_rumble_ram_battery = 0x1e,
        mbc3_timer_battery = 0x0f,
        mbc3_timer_ram_battery = 0x10,
        mbc3 = 0x11,
        mbc3_ram = 0x12,
        mbc3_ram_battery = 0x13,
    };
}

#endif