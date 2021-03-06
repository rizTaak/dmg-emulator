#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <string>
#include <vector>
#include <logging/logging.h>
#include <emulator/common.h>
#include <emulator/joypad.h>
#include <emulator/mem_registers.h>
#include <emulator/cartridge.h>

namespace emulator {

    class MemoryBank {
    public:
        MemoryBank() = default;
        MemoryBank(const MemoryBank &other) = delete;
        MemoryBank(const MemoryBank &&other) = delete;
        MemoryBank &operator=(const MemoryBank &) = delete;
        MemoryBank &operator=(MemoryBank &&) = delete;

        virtual register_8_t read_byte(register_16_t address) = 0;

        virtual register_8_t &get_ref_byte(register_16_t address) = 0;

        virtual void write_byte(register_16_t address, register_8_t value) = 0;

        register_16_t read_word(register_16_t address);

        void write_word(register_16_t address, register_16_t value);

        virtual ~MemoryBank() = default;
    };

    class RomBank : public MemoryBank {
    protected:
        spdlog::logger log;
        std::vector<register_8_t> m_buffer;
        register_16_t m_offset;
    public:
        RomBank(const RomBank &other) = delete;
        RomBank(const RomBank &&other) = delete;
        RomBank &operator=(const RomBank &) = delete;
        RomBank &operator=(RomBank &&) = delete;

        RomBank(register_16_t size, register_16_t offset);

        register_8_t read_byte(register_16_t address) override;

        register_8_t &get_ref_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;
    };

    class CartridgeBank : public RomBank {
        friend class Cartridge;

        friend class Mbc0Cartridge;

        friend class Mbc1Cartridge;

        friend class Mbc5Cartridge;

        friend class Mbc3Cartridge;

        Cartridge *m_cartridge{nullptr};
    public:
        CartridgeBank(const CartridgeBank &other) = delete;
        CartridgeBank(const CartridgeBank &&other) = delete;
        CartridgeBank &operator=(const CartridgeBank &) = delete;
        CartridgeBank &operator=(CartridgeBank &&) = delete;

        CartridgeBank(const std::string &file_name, register_16_t offset);

        void load_rom(const std::string &rom_file);

        cartridge_type cart_type();

        inline bool is_valid(const register_16_t address) const {
            return m_cartridge->is_valid(address);
        }

        register_8_t read_byte(register_16_t address) override {
            return m_cartridge->read_byte(address);
        }

        register_8_t &get_ref_byte(register_16_t address) override {
            throw std::runtime_error("ref in cartridge not allowed");
        }

        void write_byte(register_16_t address, register_8_t value) override {
            m_cartridge->write_byte(address, value);
        }

        bool has_valid_cartridge() {
            return m_buffer.size() > 0;
        }

        ~CartridgeBank() override = default;
    };

    class RamBank : public RomBank {
    public:
        RamBank(const RamBank &other) = delete;
        RamBank(const RamBank &&other) = delete;
        RamBank &operator=(const RamBank &) = delete;
        RamBank &operator=(RamBank &&) = delete;

        inline bool is_valid(const register_16_t address) {
            return m_offset <= address && address < (m_offset + m_buffer.size());
        }

        explicit RamBank(register_16_t size, register_16_t offset);

        void clear_memory();

        void write_byte(register_16_t address, register_8_t value) override;
    };

    class DualMappedRamBank : public RamBank {
        register_16_t m_second_offset;
        register_16_t m_second_size;
    public:
        DualMappedRamBank(const DualMappedRamBank &other) = delete;
        DualMappedRamBank(const DualMappedRamBank &&other) = delete;
        DualMappedRamBank &operator=(const DualMappedRamBank &) = delete;
        DualMappedRamBank &operator=(DualMappedRamBank &&) = delete;

        explicit DualMappedRamBank(register_16_t size, register_16_t offset,
                                   register_16_t second_size, register_16_t second_offset);

        inline bool is_valid_second(register_16_t address) const {
            return m_second_offset <= address && address < (m_second_offset + m_second_size);
        }

        register_8_t read_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;

        register_16_t adjust(register_16_t address) {
            if (address >= m_second_offset) {
                address = address - m_second_offset + m_offset;
            }
            return address;
        }
    };

    class Memory;
    class Apu;

    class IoRamBank : public RamBank {
        Memory &m_mem;
        JoyPad *m_joy_pad;
        Cpu *m_cpu;
        Apu *m_apu;
    public:
        IoRamBank(const IoRamBank &other) = delete;
        IoRamBank(const IoRamBank &&other) = delete;
        IoRamBank &operator=(const IoRamBank &) = delete;
        IoRamBank &operator=(IoRamBank &&) = delete;

        IoRamBank(Memory &mem, register_16_t size, register_16_t offset);

        void connect(Cpu *cpu) {
            m_cpu = cpu;
        }

        void write_byte(register_16_t address, register_8_t value) override;

        register_8_t read_byte(register_16_t address) override;

        // io bank should map to devices
        void connect_joy_pad(JoyPad *joy_pad) {
            m_joy_pad = joy_pad;
        }

        void connect_apu(Apu *apu) {
            m_apu = apu;
        }
    };

    class VideoRamBank : public RamBank {
    public:
        VideoRamBank(const VideoRamBank &other) = delete;
        VideoRamBank(const VideoRamBank &&other) = delete;
        VideoRamBank &operator=(const VideoRamBank &) = delete;
        VideoRamBank &operator=(VideoRamBank &&) = delete;

        explicit VideoRamBank(register_16_t size, register_16_t offset);
    };

    class Memory {
        CartridgeBank m_boot_rom;
        CartridgeBank m_cartridge_rom;
        VideoRamBank m_video_ram;
        RamBank m_switchable_ram;
        DualMappedRamBank m_internal_ram;
        RamBank m_sprite_ram;
        RamBank m_empty_bus_ram;
        IoRamBank m_io_ram;
        RamBank m_empty_bus2_ram;
        RamBank m_internal2_ram;

        register_8_t &m_boot_disabled;
    public:
        explicit Memory(const std::string &boot_rom, const std::string &game_rom);

        Memory(const Memory &other) = delete;
        Memory(const Memory &&other) = delete;
        Memory &operator=(const Memory &) = delete;
        Memory &operator=(Memory &&) = delete;

        register_8_t read_byte(register_16_t address);

        register_8_t &get_ref_byte(register_16_t address);

        void write_byte(register_16_t address, register_8_t value);

        register_16_t read_word(register_16_t address);

        void write_word(register_16_t address, register_16_t value);

        void self_test();

        void connect_joy_pad(JoyPad *joy_pad);
        void connect_apu(Apu *apu);

        void connect(Cpu *cpu) {
            m_io_ram.connect(cpu);
        }

        void load_game_rom(const std::string &game_rom);
        bool valid_boot_rom();
        void clear_memory();

    private:
        MemoryBank &get_bank(register_16_t address);
    };
}
#endif