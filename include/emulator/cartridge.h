#ifndef EMULATOR_CARTRIDGE_H
#define EMULATOR_CARTRIDGE_H

#include <emulator/common.h>


namespace emulator {
    class CartridgeBank;

    class Cartridge {
    protected:
        CartridgeBank &m_bank;
        cartridge_type m_type;
        size_t m_rom_size;
        size_t m_ram_size;
    public:
        explicit Cartridge(CartridgeBank &bank);

        virtual bool is_valid(register_16_t address);

        virtual register_8_t read_byte(register_16_t address) = 0;

        virtual register_8_t &get_ref_byte(register_16_t address) = 0;

        virtual void write_byte(register_16_t address, register_8_t value) = 0;

        virtual register_16_t read_word(register_16_t address);

        virtual void write_word(register_16_t address, register_16_t value);

        virtual ~Cartridge() = default;

        cartridge_type cart_type();

        size_t rom_size();

        size_t ram_size();

        virtual bool has_ram() = 0;

        virtual bool has_battery() = 0;
    };

    class Mbc0Cartridge : public Cartridge {
    public:
        explicit Mbc0Cartridge(CartridgeBank &bank);

        register_8_t read_byte(register_16_t address) override;

        register_8_t &get_ref_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;

        bool has_ram() override { return false; };

        bool has_battery() override { return false; };
    };

    enum banking_mode {
        rom_banking_mode,
        ram_banking_mode
    };

    class Mbc1Cartridge : public Cartridge {
        banking_mode m_banking_mode;
        bool m_ram_enabled;
        size_t m_5bit_bank;
        size_t m_2bit_bank;
        register_8_t *m_ram{};

        bool can_access_ram(register_16_t address);

        size_t  combined_bank();

    public:
        explicit Mbc1Cartridge(CartridgeBank &bank);

        register_8_t read_byte(register_16_t address) override;

        register_8_t &get_ref_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;

        ~Mbc1Cartridge() override {
            delete[] m_ram;
        }

        bool has_ram() override { return m_ram_size > 0; };

        bool has_battery() override { return cart_type() == cartridge_type::mbc1_ram_battery; };
    };

    class Mbc5Cartridge : public Cartridge {
        bool m_ramg;
        size_t m_romb0;
        size_t m_romb1;
        size_t m_ramb;
        register_8_t *m_ram{};

        bool can_access_ram(register_16_t address);

        size_t  combined_rom_bank();

    public:
        explicit Mbc5Cartridge(CartridgeBank &bank);

        register_8_t read_byte(register_16_t address) override;

        register_8_t &get_ref_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;

        ~Mbc5Cartridge() override {
            delete[] m_ram;
        }

        bool has_ram() override { return m_ram_size > 0; };

        bool has_battery() override;
    };

    class Mbc3Cartridge : public Cartridge {
        size_t m_latch_value;
        size_t m_ram_enabled;
        size_t m_rom_bank;
        size_t m_ram_bank;
        register_8_t *m_ram{};

    public:
        explicit Mbc3Cartridge(CartridgeBank &bank);

        register_8_t read_byte(register_16_t address) override;

        register_8_t &get_ref_byte(register_16_t address) override;

        void write_byte(register_16_t address, register_8_t value) override;

        ~Mbc3Cartridge() override {
            delete[] m_ram;
        }

        bool has_ram() override { return m_ram_size > 0; };

        bool has_battery() override;
    };
}

#endif