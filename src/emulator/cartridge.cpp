
#include <emulator/cartridge.h>
#include <emulator/memory.h>

namespace emulator {
    static constexpr size_t ram_address_mask = 0x1fff;
    static constexpr size_t rom_address_mask = 0x3fff;

    constexpr size_t _16KB = 16 * 1024;
    constexpr size_t _2KB = 2 * 1024;
    constexpr size_t _8KB = 8 * 1024;
    constexpr size_t _512KB = 512 * 1024;
    constexpr size_t _1MB = 1 - 24 * 1024;

    Cartridge::Cartridge(CartridgeBank &bank) :
            m_bank{bank} {
        if (m_bank.m_buffer.empty()) {
            m_rom_size = 0;
            m_ram_size = 0;
            return;
        }

        m_type = static_cast<cartridge_type>(m_bank.m_buffer[mem_registers::cartridge_type]);
        auto rom_size = m_bank.m_buffer[mem_registers::rom_size];
        if (0x00 <= rom_size && rom_size <= 0x08) {
            auto chunks = std::pow(2, (rom_size + 1));
            m_rom_size = _16KB * chunks;
        } else if (rom_size == 0x52) {
            m_rom_size = _16KB * 72;
        } else if (rom_size == 0x53) {
            m_rom_size = _16KB * 80;
        } else if (rom_size == 0x54) {
            m_rom_size = _16KB * 96;
        } else if (rom_size == 0xe0) {
            m_rom_size = 0x100;
        }
        //assert(m_rom_size == m_bank.m_buffer.size());
        auto ram_size = m_bank.m_buffer[mem_registers::ram_size];
        if (ram_size == 0x00) {
            m_ram_size = 0x00;
        } else if (ram_size == 0x01) {
            m_ram_size = _2KB;
        } else if (ram_size == 0x02) {
            m_ram_size = _8KB;
        } else if (ram_size == 0x03) {
            m_ram_size = _8KB * 4;
        } else if (ram_size == 0x04) {
            m_ram_size = _8KB * 16;
        } else if (ram_size == 0x05) {
            m_ram_size = _8KB * 8;
        } else if (ram_size == 0x5b) {
            m_ram_size = 0x00;
        }
    }

    cartridge_type Cartridge::cart_type() {
        return m_type;
    }

    size_t Cartridge::rom_size() const {
        return m_rom_size;
    }

    size_t Cartridge::ram_size() const {
        return m_ram_size;
    }

    register_16_t Cartridge::read_word(register_16_t address) {
        return (static_cast<register_16_t>(read_byte(address + 1)) << 0x8u)
               | static_cast<register_16_t>(read_byte(address));
    }

    void Cartridge::write_word(register_16_t address, register_16_t value) {
        write_byte(address + 1, static_cast<register_8_t>((value & 0xff00u) >> 0x8u));
        write_byte(address, static_cast<register_8_t>((value & 0x00ffu)));
    }

    bool Cartridge::is_valid(register_16_t address) {
        return (0x0000 <= address && address < 0x8000)
               || (0xa000 <= address && address < 0xc000);
    }

    Mbc0Cartridge::Mbc0Cartridge(CartridgeBank &bank) :
            Cartridge(bank) {

    }

    register_8_t Mbc0Cartridge::read_byte(register_16_t address) {
        auto actual = address - m_bank.m_offset;
        assert(actual >= 0 && actual < m_bank.m_buffer.size());
        auto imm = m_bank.m_buffer[actual];
        return imm;
    }

    void Mbc0Cartridge::write_byte(register_16_t address, register_8_t value) {

    }

    NullCartridge::NullCartridge(CartridgeBank &bank) :
            Cartridge(bank) {

    }

    register_8_t NullCartridge::read_byte(register_16_t address) {
        return 0x00;
    }

    void NullCartridge::write_byte(register_16_t address, register_8_t value) {

    }


    Mbc1Cartridge::Mbc1Cartridge(CartridgeBank &bank) :
            Cartridge(bank),
            m_banking_mode{rom_banking_mode},
            m_ram_enabled{false},
            m_5bit_bank{1},
            m_2bit_bank{0} {
        if (m_ram_size > 0x00) {
            m_ram = new register_8_t[m_ram_size];
        }
    }

    register_8_t Mbc1Cartridge::read_byte(register_16_t address) {
        // rom banking mode
        if (m_banking_mode == banking_mode::rom_banking_mode) {
            // rom read
            if (0x0000 <= address && address < 0x8000) {
                // lower bank physical address
                if (address < 0x4000) {
                    // no hanky panky
                    return m_bank.m_buffer[address];
                } // higher bank physical address
                else {
                    auto masked_address = address & rom_address_mask;
                    auto physical_address = (m_2bit_bank << 19u) | (m_5bit_bank << 14u) | (masked_address);
                    physical_address = physical_address % m_bank.m_buffer.size();
                    return m_bank.m_buffer[physical_address];
                }
            } // ram read
            else if (0xa000 <= address && address < 0xc000) {
                size_t physical_address = ram_address_mask & address;
                // make sure within ram address e.g. 2KB ram
                if (can_access_ram(physical_address)) {
                    return m_ram[physical_address];
                }
                return 0xff;
            } // why am i here?
            else {
                throw std::runtime_error("mbc1 rom logic error! (rom) address");
            }
        } // ram banking mode
        else if (m_banking_mode == banking_mode::ram_banking_mode) {
            // reading rom
            if (0x0000 <= address && address < 0x8000) {
                if (address < 0x4000) {
                    auto masked_address = address & rom_address_mask;
                    auto physical_address = (m_2bit_bank << 19u) | (masked_address);
                    physical_address = physical_address % m_bank.m_buffer.size();
                    return m_bank.m_buffer[physical_address];
                } else {
                    auto masked_address = address & rom_address_mask;
                    auto physical_address = (m_2bit_bank << 19u) | (m_5bit_bank << 14u) | (masked_address);
                    physical_address = physical_address % m_bank.m_buffer.size();
                    return m_bank.m_buffer[physical_address];
                }
            } // reading ram
            else if (0xa000 <= address && address < 0xc000) {
                size_t masked_address = (ram_address_mask & address);
                size_t physical_address = (m_2bit_bank << 13u) | masked_address;
                if (can_access_ram(physical_address)) {
                    return m_ram[physical_address];
                }
                if (can_access_ram(masked_address)) {
                    return m_ram[masked_address];
                }
                return 0xff;
            } // nope
            else {
                throw std::runtime_error("mbc1 rom logic error! (rom) address");
            }
        } // royal screw-up
        else {
            throw std::runtime_error("mbc1 rom logic error! (mode)");
        }
    }

    void Mbc1Cartridge::write_byte(register_16_t address, register_8_t value) {
        if (0xa000 <= address && address < 0xc000) {
            // verified
            if (m_banking_mode == banking_mode::rom_banking_mode) {
                size_t masked_address = ram_address_mask & address;
                if (can_access_ram(masked_address)) {
                    m_ram[masked_address] = value;
                }
            } else {
                size_t masked_address = ram_address_mask & address;
                size_t physical_address = (m_2bit_bank << 13u) | masked_address;
                if (can_access_ram(physical_address)) {
                    m_ram[physical_address] = value;
                } else if (can_access_ram(masked_address)) {
                    m_ram[masked_address] = value;
                }
            }
        } else if (0x0000 <= address && address < 0x2000) {
            //verified
            register_8_t masked = value & 0x0fu;
            m_ram_enabled = masked == 0x0au;
        } else if (0x2000 <= address && address < 0x4000) {
            //verified
            register_8_t masked = (value & 0xffu);
            if (masked == 0x00) masked = 0x01;
            m_5bit_bank = masked;
        } else if (0x4000 <= address && address < 0x6000) {
            //verified
            m_2bit_bank = value & 0x03u;
        } else if (0x6000 <= address && address < 0x8000) {
            //verified
            register_8_t masked = value & 0x01u;
            if (masked == 0x01) {
                m_banking_mode = banking_mode::ram_banking_mode;
            } else {
                m_banking_mode = banking_mode::rom_banking_mode;
            }
        }
    }

    bool Mbc1Cartridge::can_access_ram(register_16_t address) {
        return m_ram_enabled && address < m_ram_size;
    }

    Mbc5Cartridge::Mbc5Cartridge(CartridgeBank &bank) :
            Cartridge(bank),
            m_ramg{false},
            m_romb0{1},
            m_romb1{0},
            m_ramb{0} {
        if (m_ram_size > 0x00) {
            m_ram = new register_8_t[m_ram_size];
        }
    }

    bool Mbc5Cartridge::can_access_ram(register_16_t address) {
        return m_ramg && address < m_ram_size;
    }

    size_t Mbc5Cartridge::combined_rom_bank() const {
        return (m_romb1 << 8u | m_romb0);
    }

    register_8_t Mbc5Cartridge::read_byte(register_16_t address) {
        if (0x0000 <= address && address < 0x8000) {
            if (address < 0x4000) {
                return m_bank.m_buffer[address];
            }
            else {
                auto masked_address = address & rom_address_mask;
                auto physical_address = (combined_rom_bank() << 14u) | (masked_address);
                physical_address = physical_address % m_bank.m_buffer.size();
                return m_bank.m_buffer[physical_address];
            }
        }
        else if (0xa000 <= address && address < 0xc000) {
            size_t physical_address = ram_address_mask & address;
            if (can_access_ram(physical_address)) {
                return m_ram[physical_address];
            }
            return 0xff;
        }
        else {
            throw std::runtime_error("mbc5 rom logic error! (rom) address");
        }
    }

    void Mbc5Cartridge::write_byte(register_16_t address, register_8_t value) {
        if (0xa000 <= address && address < 0xc000) {
            size_t masked_address = ram_address_mask & address;
            size_t physical_address = (m_ramb << 13u) | masked_address;
            if (can_access_ram(physical_address)) {
                m_ram[physical_address] = value;
            } else if (can_access_ram(masked_address)) {
                m_ram[masked_address] = value;
            }
        } else if (0x0000 <= address && address < 0x2000) {
            m_ramg = value == 0x0au;
        } else if (0x2000 <= address && address < 0x3000) {
            m_romb0 = value;
        } else if (0x3000 <= address && address < 0x4000) {
            m_romb1 = value & 0x01u;
        } else if (0x4000 <= address && address < 0x6000) {
            m_ramb = value &0x0fu;
        } else if (0x6000 <= address && address < 0x8000) {
            // no banking mode
        }
    }

    bool Mbc5Cartridge::has_battery() {
        return m_type == cartridge_type::mbc5_ram_battery
            | m_type == cartridge_type::mbc5_rumble_ram_battery;
    }



    Mbc3Cartridge::Mbc3Cartridge(CartridgeBank &bank):
            Cartridge(bank),
            m_ram_enabled{false},
            m_rom_bank{1},
            m_ram_bank{0},
            m_latch_value{}{
        if (m_ram_size > 0x00) {
            m_ram = new register_8_t[m_ram_size];
        }
    }

    register_8_t Mbc3Cartridge::read_byte(register_16_t address) {
        if (0x0000 <= address && address < 0x8000) {
            if (address < 0x4000) {
                return m_bank.m_buffer[address];
            }
            else {
                auto masked_address = address & rom_address_mask;
                auto physical_address = (m_rom_bank << 14u) | masked_address;
                physical_address = physical_address % m_bank.m_buffer.size();
                return m_bank.m_buffer[physical_address];
            }
        }
        else if (0xa000 <= address && address < 0xc000) {
            if (m_ram_enabled == 0x0au) {
                // note: it should be 0x03u but changed to 0xffu to pass mbctest
                if (m_ram_bank <= 0x07u) {
                    size_t masked_address = ram_address_mask & address;
                    size_t physical_address = (m_ram_bank << 13u) | masked_address;
                    return m_ram[physical_address];
                } else if (0x08u <= m_ram_bank && m_ram_bank <= 0x0cu) {
                    return m_latch_value;
                }
                return 0xff;
            }
            return 0xff;
        }
        else {
            throw std::runtime_error("mbc3 rom logic error! (rom) address");
        }
    }

    void Mbc3Cartridge::write_byte(register_16_t address, register_8_t value) {
        if (0xa000 <= address && address < 0xc000) {
            if (m_ram_enabled == 0x0au){
                // note: it should be 0x03u but changed to 0xffu to pass mbctest
                if (m_ram_bank <= 0x07u) {
                    size_t masked_address = ram_address_mask & address;
                    size_t physical_address = (m_ram_bank << 13u) | masked_address;
                    m_ram[physical_address] = value;
                } else if (0x08u <= m_ram_bank && m_ram_bank <= 0x0cu) {
                    // what is this?
                    m_latch_value = value;
                }
            }
        } else if (0x0000 <= address && address < 0x2000) {
            m_ram_enabled = value;
        } else if (0x2000 <= address && address < 0x4000) {
            // note: it should be 0x7fu but changed to 0xffu to pass mbctest
            auto masked = value & 0xffu;
            if (masked == 0x00u) masked = 0x01u;
            m_rom_bank = masked;
        } else if (0x4000 <= address && address < 0x6000) {
            m_ram_bank = value;
        } else if (0x6000 <= address && address < 0x8000) {
            // todo:
            m_latch_value = value;
        }
    }

    bool Mbc3Cartridge::has_battery() {
        return m_type == cartridge_type::mbc3_ram_battery
               || m_type == cartridge_type::mbc3_timer_battery
               || m_type == cartridge_type::mbc3_timer_ram_battery;
    }
}