#ifndef EMULATOR_DMG_H
#define EMULATOR_DMG_H

#include <emulator/memory.h>
#include <emulator/cpu.h>
#include <emulator/gpu.h>
#include <logging/logging.h>
#include <emulator/timer.h>
#include <emulator/joypad.h>
#include <emulator/time.h>
#include <emulator/apu.h>

namespace emulator {
    class Dmg {
        spdlog::logger log = Logger::get_logger("dmg");
        Memory m_mem;
        Cpu m_cpu;
        Gpu m_gpu;
        JoyPad m_joy_pad;
        Timer m_timer;
        Host *m_host;
        bool m_skip_boot;
        RealTime m_wall_clock;
        Apu m_apu;
        bool m_on {false};
    public:
        Dmg(const Dmg &other) = delete;
        Dmg(const Dmg &&other) = delete;
        Dmg &operator=(const Dmg &) = delete;
        Dmg &operator=(Dmg &&) = delete;

        explicit Dmg(bool skip_boot, const std::string &boot_rom,
                     const std::string &game_rom, Host *host);
        void run();
        void turn_off();
        void turn_on();
        void insert_cartridge(const std::string &game_rom);
    };
}

#endif
