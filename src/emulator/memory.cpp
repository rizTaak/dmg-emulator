
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <emulator/memory.h>
#include <spdlog/fmt/fmt.h>
#include <logging/logging.h>
#include <emulator/mem_registers.h>
#include <emulator/joypad.h>
#include <emulator/cpu.h>
#include <emulator/apu.h>

namespace emulator {
    using mstream = std::basic_ifstream<register_8_t>;

    register_16_t MemoryBank::read_word(register_16_t address) {
        return (static_cast<register_16_t>(read_byte(address + 1)) << 0x8u)
               | static_cast<register_16_t>(read_byte(address));
    }

    void MemoryBank::write_word(register_16_t address, register_16_t value) {
        write_byte(address + 1, static_cast<register_8_t>((value & 0xff00u) >> 0x8u));
        write_byte(address, static_cast<register_8_t>((value & 0x00ffu)));
    }

    RomBank::RomBank(const register_16_t size, const register_16_t offset) :
            log{Logger::get_logger("memory_bank")},
            m_buffer{},
            m_offset{offset} {
        m_buffer.resize(size);
    }

    register_8_t RomBank::read_byte(register_16_t address) {
        auto actual = address - m_offset;
        assert(actual >= 0 && actual < m_buffer.size());
        auto imm = m_buffer[actual];
        return imm;
    }

    register_8_t &RomBank::get_ref_byte(register_16_t address) {
        auto actual = address - m_offset;
        assert(actual >= 0 && actual < m_buffer.size());
        return m_buffer.at(actual);
    }

    void RomBank::write_byte(register_16_t address, register_8_t value) {
    }

    CartridgeBank::CartridgeBank(const std::string &file_name, register_16_t offset) :
            RomBank(0x0000, offset) {
        load_rom(file_name);
    }

    void CartridgeBank::load_rom(const std::string &rom_file) {
        if (m_cartridge != nullptr) {
            delete m_cartridge;
            m_cartridge = nullptr;
        }
        std::ifstream file(rom_file, std::ios::binary | std::ios::ate);
        if (!file.good()) {
            //bad order dependency
            m_buffer.resize(0);
            m_cartridge = new NullCartridge(*this);
            return;
        }
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        m_buffer.resize(size);
        if (!file.read(reinterpret_cast<char *>(m_buffer.data()), size)) {
            throw std::runtime_error(fmt::format("cannot read from rom file {}", rom_file));
        }
        //log.info("loaded {} of size {}", file_name, size);
        switch (cart_type()) {
            case cartridge_type::rom_only:
                m_cartridge = new Mbc0Cartridge(*this);
                break;
            case cartridge_type::mbc1:
            case cartridge_type::mbc1_ram:
            case cartridge_type::mbc1_ram_battery:
                //case cartridge_type::mbc3:
                m_cartridge = new Mbc1Cartridge(*this);
                break;
            case cartridge_type::mbc5:
            case cartridge_type::mbc5_ram:
            case cartridge_type::mbc5_ram_battery:
            case cartridge_type::mbc5_rumble:
            case cartridge_type::mbc5_rumble_ram:
            case cartridge_type::mbc5_rumble_ram_battery:
                m_cartridge = new Mbc5Cartridge(*this);
                break;
            case cartridge_type::mbc3_timer_battery:
            case cartridge_type::mbc3_timer_ram_battery:
            case cartridge_type::mbc3:
            case cartridge_type::mbc3_ram:
            case cartridge_type::mbc3_ram_battery:
                m_cartridge = new Mbc3Cartridge(*this);
                break;
            default:
                throw std::runtime_error(
                        fmt::format("unsupported cartridge type {}", cart_type()));
                break;
        }
    }

    cartridge_type CartridgeBank::cart_type() {
        if(!m_buffer.empty()) {
            register_8_t value = m_buffer[mem_registers::cartridge_type];
            return static_cast<emulator::cartridge_type>(value);
        } else {
            return static_cast<emulator::cartridge_type>(0);
        }
    }

    RamBank::RamBank(register_16_t size, register_16_t offset) :
            RomBank(size, offset) {
    }

    void RamBank::clear_memory() {
        memset(m_buffer.data(), 0, m_buffer.size());
    }

    void RamBank::write_byte(register_16_t address, register_8_t value) {
        auto actual = address - m_offset;
        assert(0 <= actual && actual < m_buffer.size());
        m_buffer[actual] = value;
    }

    VideoRamBank::VideoRamBank(register_16_t size, register_16_t offset) :
            RamBank(size, offset) {
    }

    DualMappedRamBank::DualMappedRamBank(register_16_t size, register_16_t offset,
                                         register_16_t second_size, register_16_t second_offset) :
            RamBank(size, offset),
            m_second_offset{second_offset},
            m_second_size{second_size} {
    }

    register_8_t DualMappedRamBank::read_byte(register_16_t address) {
        return RamBank::read_byte(adjust(address));
    }

    void DualMappedRamBank::write_byte(register_16_t address, register_8_t value) {
        RamBank::write_byte(adjust(address), value);
    }

    Memory::Memory(const std::string &boot_rom, const std::string &game_rom) :
            m_boot_rom{boot_rom, 0x0000},
            m_cartridge_rom{game_rom, 0x0000},
            m_video_ram{0x2000, 0x8000},
            m_switchable_ram{0x2000, 0xA000},
            m_internal_ram{0x2000, 0xC000, 0x1E00, 0xE000},
            m_sprite_ram{0x00A0, 0xFE00},
            m_empty_bus_ram{0x0060, 0xFEA0},
            m_io_ram{*this, 0x004C, 0xFF00},
            m_empty_bus2_ram{0x004C, 0xFF4C},
            m_internal2_ram{0x0080, 0xFF80},
            m_boot_disabled{get_ref_byte(mem_registers::boot_rom_disabled)} {
    }

    register_8_t Memory::read_byte(register_16_t address) {
        return get_bank(address).read_byte(address);
    }

    register_8_t &Memory::get_ref_byte(register_16_t address) {
        return get_bank(address).get_ref_byte(address);
    }

    void Memory::write_byte(register_16_t address, register_8_t value) {
        return get_bank(address).write_byte(address, value);
    }

    register_16_t Memory::read_word(register_16_t address) {
        return get_bank(address).read_word(address);
    }

    void Memory::write_word(register_16_t address, register_16_t value) {
        get_bank(address).write_word(address, value);
    }

    MemoryBank &Memory::get_bank(register_16_t address) {
        if (m_video_ram.is_valid(address)) {
            return m_video_ram;
        } else if (m_internal_ram.is_valid(address) || m_internal_ram.is_valid_second(address)) {
            return m_internal_ram;
        }
        if (m_cartridge_rom.is_valid(address)) {
            if (address < 0x0100 && m_boot_disabled == 0) {
                return m_boot_rom;
            }
            return m_cartridge_rom;
        } else if (m_internal2_ram.is_valid(address)) {
            return m_internal2_ram;
        } else if (m_switchable_ram.is_valid(address)) {
            return m_switchable_ram;
        } else if (m_sprite_ram.is_valid(address)) {
            return m_sprite_ram;
        } else if (m_empty_bus_ram.is_valid(address)) {
            return m_empty_bus_ram;
        } else if (m_io_ram.is_valid(address)) {
            return m_io_ram;
        } else if (m_empty_bus2_ram.is_valid(address)) {
            return m_empty_bus2_ram;
        }
        throw std::runtime_error(fmt::format("unknown memory address {0:#06x}", address));
    }

    void Memory::self_test() {
        register_16_t address = 0x0000;
        while (true) {
            register_16_t count = 0;
            if (m_cartridge_rom.is_valid(address)) {
                ++count;
            } else if (m_switchable_ram.is_valid(address)) {
                ++count;
            } else if (m_video_ram.is_valid(address)) {
                ++count;
            } else if (m_switchable_ram.is_valid(address)) {
                ++count;
            } else if (m_internal_ram.is_valid(address)) {
                ++count;
            } else if (m_sprite_ram.is_valid(address)) {
                ++count;
            } else if (m_empty_bus_ram.is_valid(address)) {
                ++count;
            } else if (m_io_ram.is_valid(address)) {
                ++count;
            } else if (m_empty_bus2_ram.is_valid(address)) {
                ++count;
            } else if (m_internal2_ram.is_valid(address)) {
                ++count;
            }
            if (count != 1) {
                throw std::runtime_error(fmt::format("incorrect mapping of {0:#06x} with count {1}",
                                                     address, count));
            }
            if (address == 0xffff) {
                break;
            } else {
                ++address;
            }
        }
    }

    void Memory::connect_joy_pad(JoyPad *joypad) {
        m_io_ram.connect_joy_pad(joypad);
    }

    void Memory::connect_apu(Apu *apu) {
        m_io_ram.connect_apu(apu);
    }

    void Memory::load_game_rom(const std::string &game_rom) {
        m_cartridge_rom.load_rom(game_rom);
    }

    bool Memory::valid_boot_rom() {
        return m_boot_rom.has_valid_cartridge();
    }

    void Memory::clear_memory() {
        m_video_ram.clear_memory();
        m_switchable_ram.clear_memory();
        m_internal_ram.clear_memory();
        m_sprite_ram.clear_memory();
        m_empty_bus_ram.clear_memory();
        m_io_ram.clear_memory();
        m_empty_bus2_ram.clear_memory();
        m_internal2_ram.clear_memory();
    }

    IoRamBank::IoRamBank(Memory &mem, register_16_t size, register_16_t offset) :
            RamBank(size, offset),
            m_mem{mem} {
    }

    void IoRamBank::write_byte(register_16_t address, register_8_t value) {
        switch (address) {
            case mem_registers::dma_transfer: {
                register_16_t dma_address = value << 8u;
                for (register_16_t idx = 0; idx < 0xa0; idx++) {
                    register_8_t read = m_mem.read_byte(dma_address + idx);
                    m_mem.write_byte(0xfe00 + idx, read);
                }
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::lcd_gpu_register: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::lcd_status: {
                register_8_t current = RamBank::read_byte(address);
                value = (current & 0b10000111u) | (value & 0b01111000u);
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::joypad_register: {
                value = value | 0x0fu;
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::timer_divider: {
                RamBank::write_byte(address, 0x00);
            }
                break;
            case mem_registers::current_scan_line: {
            }
                break;
            case mem_registers::scroll_x: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::scroll_y: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::coincidence_scan_line: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::ch2_nr30: {
                RamBank::write_byte(address, value & 0x80u);
            }
                break;
            case mem_registers::ch2_nr31: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::ch2_nr32: {
                RamBank::write_byte(address, value & 0b0110'0000u);
            }
                break;
            case mem_registers::ch2_nr33: {
                RamBank::write_byte(address, value);
            }
                break;
            case mem_registers::ch2_nr34: {
                bool trigger = value & 0x80u;
                RamBank::write_byte(address, value & 0b1100'0111u);
                if(trigger) {
                    m_apu->wave().trigger();
                }
            }
                break;
            case mem_registers::nr52: {
                RamBank::write_byte(address, value & 0b1000'1111u);
            }
                break;
            case mem_registers::ch0_nr10: {
                RamBank::write_byte(address, value & 0b0111'1111u);
            }
                break;
            case mem_registers::ch0_nr12: {
                RamBank::write_byte(address, value);
                m_apu->ch0().update_env();
            }
                break;
            case mem_registers::ch0_nr14: {
                bool trigger = value & 0x80u;
                RamBank::write_byte(address, value & 0b1100'0111u);
                if(trigger) {
                    m_apu->ch0().trigger();
                }
            }
                break;
            case mem_registers::ch1_nr20: {
                RamBank::write_byte(address, value & 0b0111'1111u);
            }
                break;
            case mem_registers::ch1_nr22: {
                RamBank::write_byte(address, value);
                m_apu->ch1().update_env();
            }
                break;
            case mem_registers::ch1_nr24: {
                bool trigger = value & 0x80u;
                RamBank::write_byte(address, value & 0b1100'0111u);
                if(trigger) {
                    m_apu->ch1().trigger();
                }
            }
                break;

            case mem_registers::ch3_nr42: {
                RamBank::write_byte(address, value);
                m_apu->noise().update_env();
            }
                break;
            case mem_registers::ch3_nr44: {
                bool trigger = value & 0x80u;
                RamBank::write_byte(address, value & 0b1100'0000u);
                if(trigger) {
                    m_apu->noise().trigger();
                }
            }
                break;
            default:
                RamBank::write_byte(address, value);
        }
    }

    register_8_t IoRamBank::read_byte(register_16_t address) {
        switch (address) {
            case mem_registers::joypad_register: {
                register_8_t reg_value = RamBank::read_byte(address);
                if ((reg_value & JoyPad::dir_flag) == 0) {
                    return (reg_value & 0xf0u) | (m_joy_pad->dir() & 0x0fu);
                }
                if ((reg_value & JoyPad::btn_flag) == 0) {
                    return (reg_value & 0xf0u) | (m_joy_pad->btn() & 0x0fu);
                }
                return (reg_value & 0xf0u) | 0x0fu;
            }
                break;
            default:
                return RamBank::read_byte(address);
        }
    }
}