#ifndef EMULATOR_MEM_REGISTERS_H
#define EMULATOR_MEM_REGISTERS_H

#include <emulator/common.h>

namespace emulator {
    struct mem_registers {
        static constexpr register_16_t interrupt_req_register = 0xff0fu;
        static constexpr register_16_t interrupt_enable_register = 0xffffu;

        static constexpr register_16_t timer_divider = 0xff04u;
        static constexpr register_16_t tima_timer_counter = 0xff05u; // tima
        static constexpr register_16_t tma_timer_modulo = 0xff06u; // tma
        static constexpr register_16_t tac_timer_control = 0xff07u; // tmc

        static constexpr register_16_t bank_type = 0x0147;
        static constexpr register_16_t bank_ram_size = 0x0148;

        static constexpr register_16_t lcd_gpu_register = 0xff40;
        static constexpr register_16_t lcd_status = 0xff41;
        static constexpr register_16_t scroll_y = 0xff42;
        static constexpr register_16_t scroll_x = 0xff43;
        static constexpr register_16_t current_scan_line = 0xff44;
        static constexpr register_16_t coincidence_scan_line = 0xff45;
        static constexpr register_16_t background_palette = 0xff47;
        static constexpr register_16_t window_y = 0xff4a;
        static constexpr register_16_t window_x = 0xff4b;

        static constexpr register_16_t sprite_0_palette = 0xff48;
        static constexpr register_16_t sprite_1_palette = 0xff49;

        static constexpr register_16_t dma_transfer = 0xff46;

        static constexpr register_16_t joypad_register = 0xff00;

        static constexpr register_16_t cartridge_type = 0x147;
        static constexpr register_16_t rom_size = 0x148;
        static constexpr register_16_t ram_size = 0x149;

        static constexpr register_16_t  boot_rom_disabled = 0xff50;

        static constexpr register_8_t lgr_lcd_display_bit = 0b1000'0000;
        static constexpr register_8_t lgr_window_tile_map_bit = 0b0100'0000;
        static constexpr register_8_t lgr_window_display_enable_bit = 0b0010'0000;
        static constexpr register_8_t lgr_bgw_tile_data_select_bit = 0b0001'0000;
        static constexpr register_8_t lgr_bg_tile_map_bit = 0b0000'1000;
        static constexpr register_8_t lgr_sprite_size_bit = 0b0000'0100;
        static constexpr register_8_t lgr_sprite_display_bit = 0b0000'0010;
        static constexpr register_8_t lgr_bg_display_bit = 0b0000'0001;

        static constexpr register_8_t lcds_unused_bit = 0b1000'0000;
        static constexpr register_8_t lcds_coincidence_interrupt_bit = 0b0100'0000;
        static constexpr register_8_t lcds_oam_interrupt_bit = 0b0010'0000;
        static constexpr register_8_t lcds_vblank_interrupt_bit = 0b0001'0000;
        static constexpr register_8_t lcds_hblank_interrupt_bit = 0b0000'1000;
        static constexpr register_8_t lcds_coincidence_flag_bit = 0b0000'0100;
        static constexpr register_8_t lcds_mode_mask = 0b0000'0011;

        // audios
        static constexpr register_16_t ch0_nr10 = 0xff10;
        static constexpr register_16_t ch0_nr11 = 0xff11;
        static constexpr register_16_t ch0_nr12 = 0xff12;
        static constexpr register_16_t ch0_nr13 = 0xff13;
        static constexpr register_16_t ch0_nr14 = 0xff14;

        static constexpr register_16_t ch1_nr20 = 0xff15; // not used
        static constexpr register_16_t ch1_nr21 = 0xff16;
        static constexpr register_16_t ch1_nr22 = 0xff17;
        static constexpr register_16_t ch1_nr23 = 0xff18;
        static constexpr register_16_t ch1_nr24 = 0xff19;

        static constexpr register_16_t ch2_nr30 = 0xff1a;
        static constexpr register_16_t ch2_nr31 = 0xff1b;
        static constexpr register_16_t ch2_nr32 = 0xff1c;
        static constexpr register_16_t ch2_nr33 = 0xff1d;
        static constexpr register_16_t ch2_nr34 = 0xff1e;

        static constexpr register_16_t ch3_nr41 = 0xff20;
        static constexpr register_16_t ch3_nr42 = 0xff21;
        static constexpr register_16_t ch3_nr43 = 0xff22;
        static constexpr register_16_t ch3_nr44 = 0xff23;

        static constexpr register_16_t nr50 = 0xff24;
        static constexpr register_16_t nr51 = 0xff25;
        static constexpr register_16_t nr52 = 0xff26;

        static constexpr register_16_t wave_offset = 0xff30;
    };
}

#endif