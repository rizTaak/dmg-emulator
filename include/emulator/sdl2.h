#ifndef EMULATOR_SDL2_H
#define EMULATOR_SDL2_H

#include <emulator/host.h>
#include <SDL.h>

namespace emulator {
#pragma pack(push, 1)
    struct abgr {
        register_8_t a,b,g,r;
    };
#pragma pack(pop)

    class Sdl2Host: public Host {
        SDL_Window *m_window;
        SDL_Surface *m_screen_surface;
        SDL_Texture *m_texture;
        SDL_Renderer *m_renderer;
        abgr *m_frame_buffer;
        JoyPad *m_joy_pad;
        size_t m_frames {};
        size_t m_seconds {};

        const abgr m_contrast[4] {
                {0xff,0xc6, 0xe7, 0xf7 },
                {0xff,0x48, 0x8f, 0xd6  },
                {0xff,0x24, 0x38, 0xa3  },
                {0xff,0x4f, 0x1e, 0x32 },
        };
        /*const bgra m_contrast[4] {
                {0xff, 0xff, 0xff, 0xff},
                {0xff, 0xb0, 0xb0, 0xb0},
                {0xff, 0x50, 0x50, 0x50},
                {0xff, 0x00, 0x00, 0x00},
        };*/
        const abgr *m_color_map = m_contrast;
        int m_pitch;

        void process_input();
    public:
        Sdl2Host();
        void step(clock_t ticks) override;
        void render_screen(Gpu &gpu) override;
        void connect_joy_pad(JoyPad *joy_pad) override;
        ~Sdl2Host() override;
    };
}

#endif