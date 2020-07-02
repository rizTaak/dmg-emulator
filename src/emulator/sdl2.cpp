
#include <iostream>
#include <emulator/sdl2.h>
#include <emulator/gpu.h>

namespace emulator {
    constexpr size_t size_factor = 3;

    Sdl2Host::Sdl2Host() {
        m_pitch = screen_width * sizeof(abgr);
        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("dmg", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    screen_width * size_factor, screen_height * size_factor, SDL_WINDOW_SHOWN);
        m_screen_surface = SDL_GetWindowSurface(m_window);
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        m_texture = SDL_CreateTexture(m_renderer,
                                      SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      screen_width,
                                      screen_height);
        SDL_GL_SetSwapInterval(0);
        m_frame_buffer = new abgr[screen_width * screen_height];
    }

    void Sdl2Host::step(clock_t ticks) {
    }

    void Sdl2Host::process_input() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                        case SDLK_LEFT:
                            m_joy_pad->press_left();
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            m_joy_pad->press_right();
                            break;
                        case SDLK_w:
                        case SDLK_UP:
                            m_joy_pad->press_up();
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            m_joy_pad->press_down();
                            break;
                        case SDLK_u:
                            m_joy_pad->press_select();
                            break;
                        case SDLK_i:
                            m_joy_pad->press_start();
                            break;
                        case SDLK_j:
                            m_joy_pad->press_b();
                            break;
                        case SDLK_k:
                            m_joy_pad->press_a();
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                        case SDLK_LEFT:
                            m_joy_pad->release_left();
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            m_joy_pad->release_right();
                            break;
                        case SDLK_w:
                        case SDLK_UP:
                            m_joy_pad->release_up();
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            m_joy_pad->release_down();
                            break;
                        case SDLK_u:
                            m_joy_pad->release_select();
                            break;
                        case SDLK_i:
                            m_joy_pad->release_start();
                            break;
                        case SDLK_j:
                            m_joy_pad->release_b();
                            break;
                        case SDLK_k:
                            m_joy_pad->release_a();
                            break;
                    }
                    break;

                case SDLK_ESCAPE:
                case SDL_QUIT:
                    exit(0);

                default:
                    break;
            }
        }
    }

    void Sdl2Host::render_screen(Gpu &gpu) {
        process_input();
        SDL_LockTexture(m_texture, NULL, reinterpret_cast<void **>(&m_frame_buffer), &m_pitch);
        for (size_t x = 0; x < screen_width; x++) {
            for (size_t y = 0; y < screen_height; y++) {
                m_frame_buffer[y * screen_width + x] = m_color_map[gpu.screen_buffer()[y * screen_width + x]];
            }
        }
        SDL_UnlockTexture(m_texture);
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        if(seconds != m_seconds) {
            std::string s = "dmg (" + std::to_string(m_frames) + " fps)";
            SDL_SetWindowTitle(m_window, s.c_str());
            m_seconds = seconds;
            m_frames = {};
        } else {
            m_frames++;
        }
    }

    void Sdl2Host::connect_joy_pad(JoyPad *joy_pad) {
        m_joy_pad = joy_pad;
    }

    Sdl2Host::~Sdl2Host() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}