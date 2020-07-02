#ifndef EMULATOR_APU_H
#define EMULATOR_APU_H

#include <emulator/common.h>
#include <emulator/memory.h>

namespace emulator {

    class SoundChannel {
    protected:
        const register_8_t &m_nr0;
        const register_8_t &m_nr1;
        const register_8_t &m_nr2;
        const register_8_t &m_nr3;
        const register_8_t &m_nr4;

        size_t m_timer_count{0};
        bool m_enabled{false};

        register_8_t m_output{};
    public:
        SoundChannel(const register_8_t &nr_0, const register_8_t &nr_1, const register_8_t &nr_2,
                const register_8_t &nr_3, const register_8_t &nr_4);
        void step_timer();
        [[nodiscard]] bool length_enabled() const;
        virtual void trigger() = 0;
        virtual register_8_t length_load() = 0;
        [[nodiscard]] register_8_t output() const;
    };

    class LengthCounterMixin {

    };


    class WaveChannel {
        Memory &m_mem;
        register_8_t &m_nr30;
        register_8_t &m_nr31;
        register_8_t &m_nr32;
        register_8_t &m_nr33;
        register_8_t &m_nr34;
        register_8_t &m_nr51;
        register_8_t &m_nr52;
        bool m_enabled{false};
        size_t m_timer{};
        register_8_t m_position{};
        register_16_t m_length_counter{};
        register_8_t m_output{};
        register_8_t *m_wave;
    public:
        explicit WaveChannel(Memory &mem);
        bool left_enabled();
        bool right_enabled();
        bool channel_length_status();
        bool dac_enabled();
        register_8_t length_load();
        register_8_t volume();
        register_16_t frequency();
        bool length_enabled();
        void length_step();
        void step();
        void trigger();
        register_8_t output();
    };

    class SquareChannel {
        Memory &m_mem;
        register_8_t &m_nr0;
        register_8_t &m_nr1;
        register_8_t &m_nr2;
        register_8_t &m_nr3;
        register_8_t &m_nr4;
        register_8_t &m_nr51;
        register_8_t &m_nr52;
        register_8_t m_enable_mask;
        bool m_enabled{false};
        size_t m_env_period_counter{};
        bool m_env_period_running{true};
        size_t m_sweep_period_counter{};
        bool m_sweep_enabled{false};
        register_16_t m_sweep_shadow {};
        register_8_t m_current_volume{};
        register_16_t m_length_counter{};
        register_8_t m_output{};
        static constexpr bool m_duty_lookup[4][8] = {
                {false, false, false, false, false, false, false, true},
                {true, false, false, false, false, false, false, true},
                {true, false, false, false, false, true, true, true},
                {false, true, true, true, true, true, true, false}
        };
        size_t m_timer{};
        register_8_t m_position{};
    public:
        explicit SquareChannel(Memory &mem, register_8_t &nr0, register_8_t &nr1,
                               register_8_t &nr2, register_8_t &nr3, register_8_t &nr4, register_8_t enable_mask);
        bool left_enabled();
        bool right_enabled();
        bool dac_enabled();

        register_8_t sweep_period();
        bool negate();
        register_8_t shift();

        register_8_t duty();
        register_8_t length_load();

        register_8_t starting_volume();
        bool envelope_mode();
        register_8_t env_period();

        register_16_t frequency();
        bool length_enabled();

        void length_step();
        void step();
        void env_step();
        void sweep_step();
        register_8_t output();
        void trigger();
        void update_env();
        register_16_t calc_sweep();
    };

    class NoiseChannel {
        Memory &m_mem;
        register_8_t &m_nr41;
        register_8_t &m_nr42;
        register_8_t &m_nr43;
        register_8_t &m_nr44;
        register_8_t &m_nr51;
        register_8_t &m_nr52;
        const size_t m_divisors[8] = { 8, 16, 32, 48, 64, 80, 96, 112 };
        size_t m_timer{};
        register_8_t m_output{};
        bool m_enabled{false};
        size_t m_env_period_counter{};
        bool m_env_period_running{true};
        register_8_t m_current_volume{};
        register_16_t m_lsfr{};
        register_16_t m_length_counter{};
    public:
        explicit NoiseChannel(Memory &mem);
        bool left_enabled();
        bool right_enabled();
        bool dac_enabled();

        register_8_t length_load();

        register_8_t starting_volume();
        bool env_add_mode();
        register_8_t env_period();


        register_8_t clock_shift();
        bool width_mode();
        register_8_t divisor_code();

        bool length_enabled();

        void length_step();
        void step();
        void trigger();
        register_8_t output();
        void env_step();
        void update_env();
    };

    static constexpr size_t sample_size = 4096;
    static constexpr size_t frame_seq_clocks = 8192;

    class Apu {
        Memory &m_mem;
        WaveChannel m_wave;
        SquareChannel m_ch0;
        SquareChannel m_ch1;
        NoiseChannel m_noise;
        register_8_t &m_nr50;
        register_8_t &m_nr51;
        register_8_t &m_nr52;

        size_t m_frame_seq_count = frame_seq_clocks;
        size_t m_down_sample = 95;
        size_t m_buffer_fill = 0;
        float m_audio_buffer[sample_size] = { 0 };
        register_8_t m_frame_seq = 0;
    public:
        register_8_t left_volume();
        register_8_t right_volume();
        explicit Apu(Memory &mem);
        void step(clock_t ticks);
        WaveChannel &wave();
        SquareChannel &ch0();
        SquareChannel &ch1();
        NoiseChannel &noise();
    };
}

#endif