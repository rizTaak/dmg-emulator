#ifndef EMULATOR_HOST_H
#define EMULATOR_HOST_H

#include <emulator/common.h>

namespace emulator {
    class Gpu;
    class JoyPad;

    class Host {
    public:
        virtual void step(clock_t ticks) =0;
        virtual void render_screen(Gpu &gpu) =0;
        virtual void connect_joy_pad(JoyPad *joy_pad) =0;
        virtual ~Host() =default;
    };
}

#endif