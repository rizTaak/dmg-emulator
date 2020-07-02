#include <emulator/common.h>
#include <emulator/gpu.h>
#include <emulator/cpu.h>

namespace emulator {
    TileMap::TileMap(Memory &mem, register_16_t offset) :
            m_mem{mem},
            // ASSUMPTION: memory is contiguous
            m_tile_map {&mem.get_ref_byte(offset)}{
    }

    register_8_t TileMap::tile(register_8_t x, register_8_t y) const {
        x = x >> 3u;
        y = y >> 3u;
        auto offset = (y<<5u) + x;
        return m_tile_map[offset];
    }

    TileData::TileData(Memory &mem, register_16_t offset, bool _signed):
            m_mem{mem},
            m_start_address{offset},
            m_signed{_signed},
            // ASSUMPTION: memory is contiguous
            m_tile_data{&mem.get_ref_byte(offset)}{
    }

    register_8_t TileData::tile_pixel(register_8_t tile_no, register_8_t x, register_8_t y) const {
        assert(m_start_address == 0x8000 || m_start_address == 0x8800);
        if (m_signed) {
            tile_no = (static_cast<int8_t>(tile_no) + 128);
        }
        register_16_t tile_offset = tile_no << 4u;
        register_16_t address0 = tile_offset + (y << 1u);
        register_16_t address1 = address0 + 1;
        register_8_t right_byte = m_tile_data[address0];
        register_8_t left_byte = m_tile_data[address1];
        auto left = (left_byte & (0x80u >> x)) > 0 ? 0x02 : 0x00;
        auto right = (right_byte & (0x80u >> x)) > 0 ? 0x01 : 0x00;
        register_8_t value = left + right;
        return value;
    }

    Gpu::Gpu(Memory &mem) :
            m_mem{mem},
            m_tile_map0 {m_mem, 0x9800},
            m_tile_map1 {m_mem, 0x9C00},
            m_tile_data0 {m_mem, 0x8800, true},
            m_tile_data1 {m_mem, 0x8000},
            m_sprites{},
            m_bg_palette {m_mem, mem_registers::background_palette},
            m_sprite_0_palette {m_mem, mem_registers::sprite_0_palette},
            m_sprite_1_palette {m_mem, mem_registers::sprite_1_palette},
            m_lcd_gpu_register {m_mem.get_ref_byte(mem_registers::lcd_gpu_register)},
            m_scroll_x {m_mem.get_ref_byte(mem_registers::scroll_x)},
            m_scroll_y {m_mem.get_ref_byte(mem_registers::scroll_y)},
            m_scan_line {m_mem.get_ref_byte(mem_registers::current_scan_line)},
            m_lcd_status {m_mem.get_ref_byte(mem_registers::lcd_status)},
            m_coincidence_scan_line {m_mem.get_ref_byte(mem_registers::coincidence_scan_line)},
            m_window_x {m_mem.get_ref_byte(mem_registers::window_x)},
            m_window_y {m_mem.get_ref_byte(mem_registers::window_y)}{
        m_lcd_status = 0b1000'0000;
        set_lcd_mode(screen_mode::oam);
        m_valid_y_sprites.reserve(40);
        for(int x=0; x<160; x++) {
            for(int y=0; y<144; y++) {
                m_gpu_buffer[y*160+x] = 0xff;
            }
        }
        for(register_16_t idx=0; idx<40; idx++) {
            m_sprites.emplace_back(*this, m_mem, idx);
        }

    }

    void Gpu::step(clock_t ticks) {
        if(lcd_enabled()) {
            m_accum_ticks += ticks;
            switch (get_lcd_mode()) {
                case screen_mode::oam:
                    if (m_accum_ticks >= 80) {
                        m_accum_ticks -= 80;
                        set_lcd_mode(screen_mode::vram);
                    }
                    break;
                case screen_mode::vram:
                    if (m_accum_ticks >= 172) {
                        m_accum_ticks -= 172;
                        render_scan_line();
                        set_lcd_mode(screen_mode::hblank);
                    }
                    break;
                case screen_mode::hblank:
                    if (m_accum_ticks >= 204) {
                        m_accum_ticks -= 204;
                        if (scan_line() == 143) {
                            set_lcd_mode(screen_mode::vblank);
                            m_window_curr_line = 0;
                            m_cpu->vblank_set_interrupt();
                            this->update_frame_buffer();
                        } else {
                            set_lcd_mode(screen_mode::oam);
                        }
                        set_scan_line(scan_line() + 1);
                    }
                    break;
                case screen_mode::vblank:
                    if (m_accum_ticks >= 456) {
                        m_accum_ticks -= 456;
                        if (scan_line() == 153) {
                            set_lcd_mode(screen_mode::oam);
                            set_scan_line(0);
                        } else {
                            set_scan_line(scan_line() + 1);
                        }
                    }
                    break;
                default:
                    throw std::runtime_error("unknown lcd mode");
                    break;
            }
            check_and_raise_stat_interrupt();
        } else {
            m_accum_ticks = 0 ;
            set_scan_line(0);
            set_lcd_mode(screen_mode::hblank);
        }
    }

    void Gpu::set_scan_line(register_8_t value) {
        m_scan_line = value;
        if(m_scan_line == m_coincidence_scan_line) {
            set_coincidence_bit();
        } else {
            clear_coincidence_bit();
        }
    }

    void Gpu::render_scan_line() {
        bool inc_window = false;
        m_used_sprites.clear();
        auto y = scan_line();
        auto y_scroll_val = y_scroll();
        //log.info(
        //        "render-scan-line: y:{0:#02x} y_scroll:{1:#02x} x_scroll:{2:#02x}, "
        //        "scan_line:{3:#02x} coinc_line:{4:#02x} lcd_gpu:{5:#010b} lcd_status:{6:#010b}",
        //            y, y_scroll_val, x_scroll(), m_scan_line,
        //            m_coincidence_scan_line, m_lcd_gpu_register, m_lcd_status);
        auto y_scrolled = y_scroll_val + y;
        m_valid_y_sprites.clear();
        for(auto &sprite: m_sprites) {
            if(sprite.valid_y(y)) {
                m_valid_y_sprites.push_back(&sprite);
            }
        }
        for (register_8_t x=0; x<160u; x++) {
            auto x_scrolled = x + x_scroll();
            if(background_display_enabled()) {
                auto bmi = background_tile_map_idx();
                auto tile_no = tile_map(bmi).tile(x_scrolled, y_scrolled);
                auto tdi = tile_data_idx();
                auto bg_color_idx = tile_data(tdi).tile_pixel(tile_no, x_scrolled%8, y_scrolled%8);
                auto color = m_bg_palette.get_color(bg_color_idx);
                m_gpu_buffer[y*160+x] = color;
            } else {
                m_gpu_buffer[y*160+x] = m_bg_palette.get_color(white_color);
            }
            if(window_enabled()){
                int window_x = m_window_x - 7;
                if (x >= window_x && y >= m_window_y) {
                    inc_window = true;
                    window_x = x - window_x;
                    if(window_x >= 0) {
                        auto wmi = window_tile_map_idx();
                        auto tile_no = tile_map(wmi).tile(window_x, m_window_curr_line);
                        auto tdi = tile_data_idx();
                        auto color_idx = tile_data(tdi).tile_pixel(tile_no, window_x % 8, m_window_curr_line % 8);
                        auto color = m_bg_palette.get_color(color_idx);
                        m_gpu_buffer[y * 160 + x] = color;
                    }
                }
            }
            if(sprites_enabled()){
                std::sort(m_valid_y_sprites.begin(), m_valid_y_sprites.end(),
                        +[](Sprite* left, Sprite* right){
                    if (left->x_loc() == right->x_loc()) {
                        return left < right;
                    }
                    return left->x_loc() < right->x_loc();
                });
                for(int sprite_idx=0; sprite_idx < m_valid_y_sprites.size(); sprite_idx++) {
                    auto sprite = m_valid_y_sprites[sprite_idx];
                    // only 10 sprites are allowed
                    if(m_used_sprites.size() == 10
                        && m_used_sprites.find(sprite) == m_used_sprites.end()) {
                        continue;
                    }
                    if(sprite->valid_x(x)) {
                        auto [sprite_idx, sprite_color] = sprite->get_color(x, y);
                        auto bg_color = m_gpu_buffer[y*160+x];
                        if(sprite->render_priority()) {
                            // draw on top
                            if (sprite_idx != white_color) {
                                m_gpu_buffer[y * 160 + x] = sprite_color;
                                m_used_sprites.insert(sprite);
                                break;
                            }
                        } else {
                            // draw at back
                            if(bg_color == white_color)
                            {
                                m_gpu_buffer[y * 160 + x] = sprite_color;
                                m_used_sprites.insert(sprite);
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(inc_window) {
            m_window_curr_line++;
        }
    }

    void Gpu::update_frame_buffer() {
        m_host->render_screen(*this);
    }

    void Gpu::dump_tiles(register_8_t idx) {
#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_TRACE
        register_16_t offset = idx == 0 ? 0x8800 : 0x8000;
        log.info("0xff40={0:#010b}", m_mem.read_byte(0xff40));
        for(int tile_idx=0; tile_idx<=0xff; tile_idx++) {
            log.info("{0:x}-{1:x}", idx, tile_idx);
            register_16_t  tile_offset = offset + tile_idx*16;
            log.info("{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}"
                     "{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}{:#02x}",
                     m_mem.read_byte(tile_offset+0), m_mem.read_byte(tile_offset+1),
                     m_mem.read_byte(tile_offset+2), m_mem.read_byte(tile_offset+3),
                     m_mem.read_byte(tile_offset+4),m_mem.read_byte(tile_offset+5),
                     m_mem.read_byte(tile_offset+6),m_mem.read_byte(tile_offset+7),
                     m_mem.read_byte(tile_offset+8),m_mem.read_byte(tile_offset+9),
                     m_mem.read_byte(tile_offset+10),m_mem.read_byte(tile_offset+11),
                     m_mem.read_byte(tile_offset+12), m_mem.read_byte(tile_offset+13),
                     m_mem.read_byte(tile_offset+14),m_mem.read_byte(tile_offset+15));
        }
#endif
    }

    bool Sprite::valid_x(register_8_t x) const {
        int16_t sprite_x = x_loc();
        int16_t x_left = sprite_x-8;
        int16_t x_right = x_left+8;
        return x_left <= x && x < x_right;
    }

    bool Sprite::valid_y(register_8_t y) const {
        int16_t sprite_y = y_loc();
        int16_t y_top = sprite_y-16;
        int16_t y_bottom = y_top+(m_gpu.sprite_16_enabled() ? 16 : 8);
        return y_top <= y && y < y_bottom;
    }

    std::tuple<register_8_t, register_8_t>  Sprite::get_color(register_8_t x, register_8_t y) {
        int16_t x_left = x_loc()-8;
        int16_t y_top = y_loc()-16;
        // to tile coordinates
        int16_t tile_x = x - x_left;
        int16_t tile_y = y - y_top;
        // flip
        static register_8_t flip_8_map[] = {7, 6, 5, 4, 3, 2, 1, 0};
        static register_8_t flip_16_map[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        if (x_flip()) {
            tile_x = flip_8_map[tile_x];
        }
        if (y_flip()) {
            if(m_gpu.sprite_16_enabled()) {
                tile_y = flip_16_map[tile_y];
            } else {
                tile_y = flip_8_map[tile_y];
            }
        }
        //assert(0<= tile_x && tile_x <8 && 0<= tile_y && tile_y < 8);
        // get color
        auto tile_idx = tile_no();
        if(m_gpu.sprite_16_enabled()) {
            tile_idx = tile_idx & 0xfeu;
        }
        auto color_idx = m_gpu.tile_data(1).tile_pixel(tile_idx, tile_x, tile_y);
        register_8_t color{};
        if(palette()) {
            // palette 1
            color = m_gpu.m_sprite_1_palette.get_color(color_idx);
        } else {
            // palette 0
            color = m_gpu.m_sprite_0_palette.get_color(color_idx);
        }
        return {color_idx, color};
    }

    register_8_t Palette::get_color(register_8_t color_idx) const {
        register_8_t palette = get_palette();
        register_8_t result = color_idx;
        switch(color_idx) {
            case 0x00:
                result = (0b0000'0011u & palette);
                break;
            case 0x01:
                result = (0b0000'1100u & palette) >> 2u;
                break;
            case 0x02:
                result = (0b0011'0000u & palette) >> 4u;
                break;
            case 0x03:
                result = (0b1100'0000u & palette) >> 6u;
                break;
            default:
                throw std::runtime_error("unknown color idx");
        }
        return result;
    }


    void Gpu::set_lcd_mode(screen_mode mode) {
        auto before = m_lcd_status;
        auto updated = (m_lcd_status & ~mem_registers::lcds_mode_mask)
                | (static_cast<register_8_t>(mode) & mem_registers::lcds_mode_mask);
        m_lcd_status = updated;// m_mem.write_byte(mem_registers::lcd_status, updated);
        //log.info("lcd mode: {0:#010b} {1:#02x} {2:#02x} {3:#010b}",
        //        m_lcd_status, m_scan_line, m_coincidence_scan_line, before);
    }

    void Gpu::raise_lcd_interrupt() {
        m_cpu->lcdc_set_interrupt();
    }

    void Gpu::check_and_raise_stat_interrupt() {
        auto mode = get_lcd_mode();
        bool new_eval = ((m_scan_line == m_coincidence_scan_line) && coincidence_interrupt_enabled())
                || ((mode == screen_mode::hblank) && hblank_interrupt_enabled())
                || ((mode == screen_mode::vblank) && vblank_interrupt_enabled())
               || ((mode == screen_mode::oam) && oam_interrupt_enabled());
        if(!m_last_stat_eval && new_eval) {
            raise_lcd_interrupt();
        }
        m_last_stat_eval = new_eval;
    }
}