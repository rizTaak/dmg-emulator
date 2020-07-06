#ifndef EMULATOR_GPU_H
#define EMULATOR_GPU_H

#include <set>
#include <tuple>
#include <logging/logging.h>
#include <emulator/memory.h>
#include <emulator/mem_registers.h>
#include <emulator/host.h>

namespace emulator {
    class Cpu;
    class Gpu;

    static constexpr register_8_t white_color = 0x00;
    static constexpr size_t screen_width = 160;
    static constexpr size_t screen_height = 144;

    class Palette {
        register_8_t &m_palette;
        [[nodiscard]] inline register_8_t get_palette() const {
            return m_palette;
        }
    public:
        Palette(Memory &mem, register_16_t offset):
                m_palette {mem.get_ref_byte(offset)}{
        }

        register_8_t get_color(register_8_t color_idx) const;
    };

    class Sprite {
        Gpu &m_gpu;
        register_8_t &m_x;
        register_8_t &m_y;
        register_8_t &m_tile_no;
        register_8_t &m_bits;
    public:
        Sprite(Gpu &gpu, Memory &mem, register_8_t index):
            m_gpu {gpu},
            m_x {mem.get_ref_byte(static_cast<register_16_t>((0xfe00+index*4)+1))},
            m_y {mem.get_ref_byte(static_cast<register_16_t>((0xfe00+index*4)))},
            m_tile_no {mem.get_ref_byte(static_cast<register_16_t>((0xfe00+index*4)+2))},
            m_bits {mem.get_ref_byte(static_cast<register_16_t>((0xfe00+index*4)+3))} {
        }

        [[nodiscard]] inline register_8_t y_loc() const {
            return m_y;
        }

        [[nodiscard]] inline register_8_t x_loc() const {
            return m_x;
        }

        [[nodiscard]] inline register_8_t tile_no() const{
            return m_tile_no;
        }

        [[nodiscard]] inline bool render_priority() const {
            return (m_bits & 0b1000'0000u) == 0x00;
        }

        [[nodiscard]] inline bool y_flip() const {
            return (m_bits & 0b0100'0000u) > 0x00u;
        }

        [[nodiscard]] inline bool x_flip() const {
            return (m_bits & 0b0010'0000u) > 0x00u;
        }

        [[nodiscard]] inline bool palette() const {
            return m_bits & 0b0001'0000u;
        }

        [[nodiscard]] bool valid_x(register_8_t x) const;
        [[nodiscard]] bool valid_y(register_8_t y) const;
        std::tuple<register_8_t, register_8_t> get_color(register_8_t x, register_8_t y);
    };

    class TileMap {
        Memory& m_mem;
        register_8_t *m_tile_map;
    public:
        TileMap(Memory& mem, register_16_t offset);
        register_8_t tile(register_8_t x, register_8_t y) const;
    };

    class TileData {
        Memory& m_mem;
        register_16_t m_start_address;
        bool m_signed;
        register_8_t *m_tile_data;
    public:
        TileData(Memory& mem, register_16_t offset, bool _signed = false);
        register_8_t tile_pixel(register_8_t tile_no, register_8_t x, register_8_t y) const;
    };

    enum screen_mode: register_8_t {
        oam = 0x02,
        vram = 0x03,
        hblank = 0x00,
        vblank = 0x01
    };

    class Gpu {
        friend class Sprite;
        spdlog::logger log = Logger::get_logger("gpu");
    protected:
        register_8_t m_gpu_buffer[screen_width * screen_height]{};
        clock_t m_accum_ticks{0};
        Memory &m_mem;
        const TileMap m_tile_map0;
        const TileMap m_tile_map1;
        const TileData m_tile_data0;
        const TileData m_tile_data1;
        std::vector<Sprite> m_sprites;
        Cpu *m_cpu {};
        const Palette m_bg_palette;
        const Palette m_sprite_0_palette;
        const Palette m_sprite_1_palette;
        const register_8_t &m_lcd_gpu_register; //ff40
        const register_8_t &m_scroll_x;
        const register_8_t &m_scroll_y;
        register_8_t &m_scan_line;
        register_8_t &m_lcd_status; //ff41
        const register_8_t &m_coincidence_scan_line;
        const register_8_t &m_window_x;
        const register_8_t &m_window_y;
        register_8_t m_window_curr_line;
        Host *m_host{};
    private:
        std::vector<Sprite*> m_valid_y_sprites{};
        std::set<Sprite*> m_used_sprites{};
        bool m_last_stat_eval {false};
    public:
        Gpu(const Gpu &other) = delete;
        Gpu(const Gpu &&other) = delete;
        Gpu &operator=(const Gpu &) = delete;
        Gpu &operator=(Gpu &&) = delete;

        explicit  Gpu(Memory &mem);
        void step(clock_t ticks);
        void dump_tiles(register_8_t idx);
        void update_frame_buffer();

        void connection_cpu(Cpu *cpu) {
            m_cpu = cpu;
        }

        void connection_host(Host *host) {
            m_host = host;
        }

        [[nodiscard]] inline bool sprite_16_enabled() const {
            return (m_lcd_gpu_register & mem_registers::lgr_sprite_size_bit);
        }

        [[nodiscard]] inline register_8_t background_tile_map_idx() const {
            return (m_lcd_gpu_register & mem_registers::lgr_bg_tile_map_bit) ? 1 : 0;
        }

        const TileData &tile_data(register_8_t idx) {
            if (idx == 0) {
                return m_tile_data0;
            } else {
                return m_tile_data1;
            }
        }

        const TileMap &tile_map(register_8_t idx) {
            if (idx == 0) {
                return m_tile_map0;
            } else {
                return m_tile_map1;
            }
        }

        [[nodiscard]] inline register_8_t tile_data_idx() const {
            return (m_lcd_gpu_register & mem_registers::lgr_bgw_tile_data_select_bit) ? 1 : 0;
        }

        const register_8_t* screen_buffer() {
            return m_gpu_buffer;
        }

        void clear_screen();
    private:
        void render_scan_line();

        [[nodiscard]] inline register_8_t scan_line() const {
            return m_scan_line;
        }

        void set_scan_line(register_8_t value);

        [[nodiscard]] inline register_8_t x_scroll() const {
            return m_scroll_x;
        }

        [[nodiscard]] inline register_8_t y_scroll() const {
            return m_scroll_y;
        }

        [[nodiscard]] inline bool lcd_enabled() const {
            return m_lcd_gpu_register & mem_registers::lgr_lcd_display_bit;
        }

        [[nodiscard]] inline register_8_t window_tile_map_idx() const {
            return (m_lcd_gpu_register & mem_registers::lgr_window_tile_map_bit) ? 1 : 0;
        }

        [[nodiscard]] inline bool window_enabled() const {
            return (m_lcd_gpu_register & mem_registers::lgr_window_display_enable_bit);
        }

        [[nodiscard]] inline bool sprites_enabled() const {
            return (m_lcd_gpu_register & mem_registers::lgr_sprite_display_bit);
        }

        [[nodiscard]] inline bool background_display_enabled() const {
            return (m_lcd_gpu_register & mem_registers::lgr_bg_display_bit);
        }

        void set_lcd_mode(screen_mode mode);

        [[nodiscard]] inline bool hblank_interrupt_enabled() const {
            return (m_lcd_status & mem_registers::lcds_hblank_interrupt_bit);
        }

        [[nodiscard]] inline bool vblank_interrupt_enabled() const {
            return (m_lcd_status & mem_registers::lcds_vblank_interrupt_bit);
        }

        [[nodiscard]] inline bool oam_interrupt_enabled() const {
            return (m_lcd_status & mem_registers::lcds_oam_interrupt_bit);
        }

        [[nodiscard]] inline bool coincidence_interrupt_enabled() const {
            return (m_lcd_status & mem_registers::lcds_coincidence_interrupt_bit) != 0;
        }

        inline void set_coincidence_bit() {
            auto updated = (m_lcd_status | mem_registers::lcds_coincidence_flag_bit);
            m_lcd_status = updated; //m_mem.write_byte(mem_registers::lcd_status, updated);
            //log.info(" set_coincidence_bit lcd mode: {0:#010b} {1:#02x} {2:#02x}",
            //         m_lcd_status, m_scan_line, m_coincidence_scan_line);
        }

        inline void clear_coincidence_bit() {
            auto updated = (m_lcd_status & ~mem_registers::lcds_coincidence_flag_bit);
            m_lcd_status = updated; //m_mem.write_byte(mem_registers::lcd_status, updated);
            //log.info("clear_coincidence_bit lcd mode: {0:#010b} {1:#02x} {2:#02x}",
            //         m_lcd_status, m_scan_line, m_coincidence_scan_line);
        }

        void raise_lcd_interrupt();

        [[nodiscard]] inline screen_mode get_lcd_mode() const {
            auto result = static_cast<screen_mode>(m_lcd_status & mem_registers::lcds_mode_mask);
            return result;
        }

        void check_and_raise_stat_interrupt();

        void clear_buffer();
    };
}

#endif