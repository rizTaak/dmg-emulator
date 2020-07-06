
#include <logging/logging.h>
#include <emulator/dmg.h>


namespace emulator {
    Dmg::Dmg(const bool skip_boot, const std::string &boot_rom,
             const std::string &game_rom,
             Host *host) :
            m_mem{boot_rom, game_rom},
            m_cpu{m_mem},
            m_gpu{m_mem},
            m_joy_pad{m_cpu},
            m_timer{m_mem, m_cpu},
            m_host{host},
            m_skip_boot{skip_boot},
            m_wall_clock{},
            m_apu{m_mem} {
        m_cpu.connect_gpu(&m_gpu);
        m_gpu.connection_cpu(&m_cpu);
        m_gpu.connection_host(m_host);
        m_mem.connect_joy_pad(&m_joy_pad);
        m_mem.connect(&m_cpu);
        m_mem.connect_apu(&m_apu);
        m_host->connect_joy_pad(&m_joy_pad);
        m_host->connect_dmg(this);
    }

    void Dmg::run() {
        SPDLOG_LOGGER_INFO(&log, "starting dmg");
        try {
            bool last = false;
            while (true) {
                bool current = m_on;
                if (current && !last) {
                    m_wall_clock.start();
                }
                if(current) {
                    m_cpu.fetch_decode_execute();
                    auto cpu_ticks = m_cpu.last_ticks();
                    m_gpu.step(cpu_ticks);
                    m_apu.step(cpu_ticks);
                    m_timer.step(cpu_ticks);
                    m_host->step(cpu_ticks);
                    m_wall_clock.sync(cpu_ticks);
                } else {
                    m_host->step(0);
                }
                last = current;
            }
        } catch (std::exception &e) {
            SPDLOG_LOGGER_ERROR(&log, e.what());
            SPDLOG_LOGGER_ERROR(&log, "pc = {}", m_cpu.current_pc());
            m_cpu.log_history();
        }
        SPDLOG_LOGGER_INFO(&log, "done dmg");
    }

    void Dmg::turn_off() {
        m_on = false;
        m_gpu.clear_screen();
        m_mem.clear_memory();
        m_apu.clear_audio();
    }

    void Dmg::turn_on() {
        m_on = true;
        if (m_mem.valid_boot_rom()) {
            m_cpu.init_for_boot();
        } else {
            m_cpu.init_for_catridge();
        }
    }

    void Dmg::insert_cartridge(const std::string &game_rom) {
        m_mem.load_game_rom(game_rom);
    }
}