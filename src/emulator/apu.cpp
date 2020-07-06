#include <emulator/apu.h>
#include <SDL.h>

namespace emulator {

    SoundChannel::SoundChannel(const register_8_t &nr_0, const register_8_t &nr_1, const register_8_t &nr_2,
            const register_8_t &nr_3, const register_8_t &nr_4):
            m_nr0 {nr_0}, m_nr1 {nr_1}, m_nr2 {nr_2}, m_nr3 {nr_3}, m_nr4 {nr_4} {
    }

    void SoundChannel::step_timer() {
        if (m_timer_count > 0 && length_enabled()) {
            m_timer_count--;
            if (m_timer_count == 0) {
                m_enabled = false;
            }
        }
    }

    bool SoundChannel::length_enabled() const {
        return m_nr4 & 0b0100'0000u;
    }

    register_8_t SoundChannel::output() const {
        return m_enabled ? m_output : 0;
    }

    WaveChannel::WaveChannel(Memory &mem) :
            m_mem{mem},
            m_nr30{m_mem.get_ref_byte(mem_registers::ch2_nr30)},
            m_nr31{m_mem.get_ref_byte(mem_registers::ch2_nr31)},
            m_nr32{m_mem.get_ref_byte(mem_registers::ch2_nr32)},
            m_nr33{m_mem.get_ref_byte(mem_registers::ch2_nr33)},
            m_nr34{m_mem.get_ref_byte(mem_registers::ch2_nr34)},
            m_nr51{m_mem.get_ref_byte(mem_registers::nr51)},
            m_nr52{m_mem.get_ref_byte(mem_registers::nr52)},
            m_wave{&m_mem.get_ref_byte(mem_registers::wave_offset)} {
    }

    bool WaveChannel::left_enabled() {
        return m_nr51 & 0x40u;
    }

    bool WaveChannel::right_enabled() {
        return m_nr51 & 0x04u;
    }

    bool WaveChannel::channel_length_status() {
        return m_nr52 & 0x04u;
    }

    bool WaveChannel::dac_enabled() {
        return m_nr30 & 0x80u;
    }

    register_8_t WaveChannel::length_load() {
        return m_nr31;
    }

    register_8_t WaveChannel::volume() {
        return m_nr32 >> 0x5u;
    }

    register_16_t WaveChannel::frequency() {
        return ((static_cast<register_16_t>(m_nr33) & 0x0007u) << 0x8u) | (static_cast<register_16_t>(m_nr34));
    }

    bool WaveChannel::length_enabled() {
        return m_nr34 & 0b0100'0000u;
    }

    void WaveChannel::length_step() {
        if (m_length_counter > 0 && length_enabled()) {
            m_length_counter--;
            if (m_length_counter == 0) {
                m_enabled = false;
            }
        }
    }

    register_8_t WaveChannel::output() {
        return m_output;
    }

    void WaveChannel::step() {
        m_timer--;
        if (m_timer == 0) {
            m_timer = (2048 - frequency()) * 2;
            m_position = (m_position + 1u) & 0x1fu;
            if (m_enabled && dac_enabled()) {
                auto pos = m_position / 2;
                register_8_t value = m_wave[pos];
                bool high = (m_position & 0x1u) == 0;
                if (high) {
                    value >>= 0x04u;
                }
                value &= 0x0fu;
                if (volume() > 0) {
                    value >>= volume() - 1u;
                } else {
                    value = 0;
                }
                m_output = value;
            } else {
                m_output = 0;
            }
        }
    }

    void WaveChannel::trigger() {
        m_enabled = true;
        if (m_length_counter == 0) {
            m_length_counter = 256;
        }
        m_timer = (2048 - frequency()) * 2;
    }

    // square
    SquareChannel::SquareChannel(Memory &mem, register_8_t &nr0, register_8_t &nr1,
            register_8_t &nr2, register_8_t &nr3, register_8_t &nr4, register_8_t enable_mask) :
            m_mem{mem},
            m_nr0{nr0},
            m_nr1{nr1},
            m_nr2{nr2},
            m_nr3{nr3},
            m_nr4{nr4},
            m_nr51{m_mem.get_ref_byte(mem_registers::nr51)},
            m_nr52{m_mem.get_ref_byte(mem_registers::nr52)},
            m_enable_mask {enable_mask}{
    }

    bool SquareChannel::dac_enabled() {
        return m_nr2 & 0xf8u;
    }

    bool SquareChannel::left_enabled() {
        return m_nr51 & (m_enable_mask << 0x4u);
    }

    bool SquareChannel::right_enabled() {
        return m_nr51 & (m_enable_mask);
    }

    register_8_t SquareChannel::sweep_period() {
        return (m_nr0 & 0b0111'0000u) >> 0x4u;
    }

    bool SquareChannel::negate() {
        return m_nr0 & 0b0000'1000u;
    }

    register_8_t SquareChannel::shift() {
        return m_nr0 & 0b0000'0111u;
    }

    register_8_t SquareChannel::duty() {
        return (m_nr1 & 0b1100'0000u) >> 0x6u;
    }

    register_8_t SquareChannel::length_load() {
        return (m_nr1 & 0b0011'1111u);
    }

    register_8_t SquareChannel::starting_volume() {
        return m_nr2 >> 0x4u;
    }

    bool SquareChannel::envelope_mode() {
        return (m_nr2 & 0b0000'1000u);
    }

    register_8_t SquareChannel::env_period() {
        return (m_nr2 & 0b0000'0111u);
    }


    register_16_t SquareChannel::frequency() {
        return ((static_cast<register_16_t>(m_nr4) & 0x0007u) << 0x8u) | (static_cast<register_16_t>(m_nr3));
    }

    bool SquareChannel::length_enabled() {
        return m_nr4 & 0b0100'0000u;
    }

    register_8_t SquareChannel::output() {
        return m_output;
    }

    void SquareChannel::step() {
        m_timer--;
        if (m_timer == 0) {
            m_timer = (2048 - frequency()) * 4;
            m_position = (m_position + 1u) & 0x7u;
        }

        if (m_enabled && dac_enabled()) {
            m_output = m_current_volume;
        }
        else {
            m_output = 0;
        }

        if (!m_duty_lookup[duty()][m_position]) {
            m_output = 0;
        }
    }

    void SquareChannel::length_step() {
        if (m_length_counter > 0 && length_enabled()) {
            m_length_counter--;
            if (m_length_counter == 0) {
                m_enabled = false;
            }
        }
    }

    void SquareChannel::env_step() {
        m_env_period_counter--;
        if(m_env_period_counter == 0) {
            m_env_period_counter = env_period();
            if (m_env_period_counter == 0) {
                m_env_period_counter = 8;
            }
            if (m_env_period_running && env_period() > 0) {
                if (envelope_mode() && starting_volume() < 15) {
                    m_current_volume++;
                }
                else if (!envelope_mode() && starting_volume() > 0) {
                    m_current_volume--;
                }
            }
            if (m_current_volume == 0 || m_current_volume == 15) {
                m_env_period_running = false;
            }
        }
    }

    void SquareChannel::sweep_step() {
        m_sweep_period_counter--;
        if (m_sweep_period_counter == 0) {
            m_sweep_period_counter = sweep_period();
            if (m_sweep_period_counter == 0) {
                m_sweep_period_counter = 8;
            }
            if (m_sweep_enabled && sweep_period() > 0) {
                 register_16_t freq = calc_sweep();
                if (freq <= 2047 && shift() > 0) {
                    m_sweep_shadow = freq;
                    m_nr3 = static_cast<register_8_t>(freq & 0x00ffu);
                    m_nr4 = (m_nr4 & 0b1111'1000u) | static_cast<register_8_t>((freq >> 0x08u) & 0x0007u);
                    calc_sweep();
                }
                calc_sweep();
            }
        }
    }

    void SquareChannel::trigger() {
        m_enabled = true;
        if (m_length_counter == 0) {
            m_length_counter = 64;
        }
        m_timer = (2048 - frequency()) * 4;
        m_env_period_running = true;
        m_env_period_counter = env_period();
        m_current_volume = starting_volume();
        m_sweep_shadow = frequency();
        m_sweep_period_counter = sweep_period();
        if (m_sweep_period_counter == 0) {
            m_sweep_period_counter = 8;
        }
        m_sweep_enabled = m_sweep_period_counter > 0 || shift() > 0;
        if (shift() > 0) {
            calc_sweep();
        }
    }

    register_16_t SquareChannel::calc_sweep() {
        register_16_t freq = m_sweep_shadow >> shift();
        if (negate()) {
            freq = m_sweep_shadow - freq;
        }
        else {
            freq = m_sweep_shadow + freq;
        }
        if (freq > 2047) {
            m_enabled = false;
        }
        return freq;
    }

    void SquareChannel::update_env() {
        m_current_volume = starting_volume();
        m_env_period_counter = env_period();
    }

    // noise channel
    NoiseChannel::NoiseChannel(Memory &mem):
        m_mem {mem},
        m_nr41 {mem.get_ref_byte(mem_registers::ch3_nr41)},
        m_nr42 {mem.get_ref_byte(mem_registers::ch3_nr42)},
        m_nr43 {mem.get_ref_byte(mem_registers::ch3_nr43)},
        m_nr44 {mem.get_ref_byte(mem_registers::ch3_nr44)},
        m_nr51{m_mem.get_ref_byte(mem_registers::nr51)},
        m_nr52{m_mem.get_ref_byte(mem_registers::nr52)} {
    }


    bool NoiseChannel::left_enabled() {
        return m_nr51 & 0x80u;
    }

    bool NoiseChannel::right_enabled() {
        return m_nr51 & 0x08u;
    }

    bool NoiseChannel::dac_enabled() {
        return (m_nr42 & 0xf8u) != 0;
    }

    register_8_t NoiseChannel::length_load() {
        return m_nr41 & 0b0011'1111u;
    }

    register_8_t NoiseChannel::starting_volume() {
        return (m_nr42 & 0xf0u) >> 0x4u;
    }

    bool NoiseChannel::env_add_mode() {
        return m_nr42 & 0b0000'1000u;
    }

    register_8_t NoiseChannel::env_period() {
        return m_nr42 & 0b0000'0111u;
    }

    bool NoiseChannel::length_enabled() {
        return m_nr44 & 0b0100'0000u;
    }

    register_8_t NoiseChannel::clock_shift() {
        return (m_nr43 & 0xf0u) >> 0x4u;
    }

    bool NoiseChannel::width_mode() {
        return m_nr43 & 0x08u;
    }

    register_8_t NoiseChannel::divisor_code() {
        return m_nr43 & 0b0000'0111u;
    }

    void NoiseChannel::length_step() {
        if (m_length_counter > 0 && length_enabled()) {
            m_length_counter--;
            if (m_length_counter == 0) {
                m_enabled = false;
            }
        }
    }

    void NoiseChannel::step() {
        m_timer--;
        if (m_timer == 0) {
            m_timer = m_divisors[divisor_code()] << clock_shift();
            uint8_t result = (m_lsfr & 0x1) ^ ((m_lsfr >> 1) & 0x1);
            m_lsfr >>= 1;
            m_lsfr |= result << 14;
            if (width_mode()) {
                m_lsfr &= ~0x40;
                m_lsfr |= result << 6;
            }
            if (m_enabled && dac_enabled() && (m_lsfr & 0x1) == 0) {
                m_output = m_current_volume;
            }
            else {
                m_output = 0;
            }
        }
    }

    void NoiseChannel::trigger() {
        m_enabled = true;
        if (m_length_counter == 0) {
            m_length_counter = 64;
        }
        m_timer = m_divisors[divisor_code()] << clock_shift();
        m_env_period_counter = env_period();
        m_env_period_running = true;
        m_current_volume = starting_volume();
        m_lsfr = 0x7FFF;
    }

    register_8_t NoiseChannel::output() {
        return m_output;
    }

    void NoiseChannel::env_step() {
        m_env_period_counter--;
        if (m_env_period_counter == 0) {
            m_env_period_counter = env_period();
            if (m_env_period_counter == 0) {
                m_env_period_counter = 8;
            }
            if (m_env_period_running && env_period() > 0) {
                if (env_add_mode() && m_current_volume < 15) {
                    m_current_volume++;
                }
                else if (!env_add_mode() && m_current_volume > 0) {
                    m_current_volume--;
                }
            }
            if (m_current_volume == 0 || m_current_volume == 15) {
                m_env_period_running = false;
            }
        }
    }

    void NoiseChannel::update_env() {
        m_current_volume = starting_volume();
        m_env_period_counter = env_period();
    }

    Apu::Apu(Memory &mem) :
            m_mem{mem},
            m_wave{mem},
            m_ch0{mem, m_mem.get_ref_byte(mem_registers::ch0_nr10), m_mem.get_ref_byte(mem_registers::ch0_nr11),
                  m_mem.get_ref_byte(mem_registers::ch0_nr12), m_mem.get_ref_byte(mem_registers::ch0_nr13),
                  m_mem.get_ref_byte(mem_registers::ch0_nr14), 0x01u},
            m_ch1{mem, m_mem.get_ref_byte(mem_registers::ch1_nr20), m_mem.get_ref_byte(mem_registers::ch1_nr21),
                  m_mem.get_ref_byte(mem_registers::ch1_nr22), m_mem.get_ref_byte(mem_registers::ch1_nr23),
                  m_mem.get_ref_byte(mem_registers::ch1_nr24), 0x02u},
            m_noise {mem},
            m_nr50{m_mem.get_ref_byte(mem_registers::nr50)},
            m_nr51{m_mem.get_ref_byte(mem_registers::nr51)},
            m_nr52{m_mem.get_ref_byte(mem_registers::nr52)} {
        SDL_AudioSpec audioSpec;
        audioSpec.freq = 44100;
        audioSpec.format = AUDIO_F32SYS;
        audioSpec.channels = 2;
        audioSpec.samples = sample_size;
        audioSpec.callback = NULL;
        audioSpec.userdata = this;
        SDL_AudioSpec obtainedSpec;
        SDL_OpenAudio(&audioSpec, &obtainedSpec);
        SDL_PauseAudio(0);
    }

    void Apu::clear_audio() {
        SDL_ClearQueuedAudio(1);
    }

    void Apu::step(clock_t ticks) {
        while (ticks > 0) {
            m_frame_seq_count--;
            if (m_frame_seq_count == 0) {
                m_frame_seq_count = frame_seq_clocks;
                switch (m_frame_seq) {
                    case 0: {
                        m_ch0.length_step();
                        m_ch1.length_step();
                        m_wave.length_step();
                        m_noise.length_step();
                    }
                        break;
                    case 1: {
                        // nothing
                    }
                        break;
                    case 2: {
                        m_ch0.sweep_step();
                        m_ch0.length_step();
                        m_ch1.length_step();
                        m_wave.length_step();
                        m_noise.length_step();
                    }
                        break;
                    case 3: {
                        // nothing
                    }
                        break;
                    case 4: {
                        m_ch0.length_step();
                        m_ch1.length_step();
                        m_wave.length_step();
                        m_noise.length_step();
                    }
                        break;
                    case 5: {
                        // nothing
                    }
                        break;
                    case 6: {
                        m_ch0.sweep_step();
                        m_ch0.length_step();
                        m_ch1.length_step();
                        m_wave.length_step();
                        m_noise.length_step();
                    }
                        break;
                    case 7: {
                        m_ch0.env_step();
                        m_ch1.env_step();
                        m_noise.env_step();
                    }
                        break;
                }
                m_frame_seq++;
                if(m_frame_seq == 8) {
                    m_frame_seq = 0;
                }
            }

            m_ch0.step();
            m_ch1.step();
            m_wave.step();
            m_noise.step();

            m_down_sample--;
            if(m_down_sample == 0) {
                m_down_sample = 95;

                // Left
                float buffer_out = 0;
                float buffer_in = 0;
                int volume = (128*left_volume())/7;
                if (m_ch0.left_enabled()) {
                    buffer_in = ((float)m_ch0.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_ch1.left_enabled()) {
                    buffer_in = ((float)m_ch1.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_wave.left_enabled()) {
                    buffer_in = ((float)m_wave.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_noise.left_enabled()) {
                    buffer_in = ((float)m_noise.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                m_audio_buffer[m_buffer_fill] = buffer_out;

                // Right
                buffer_out = 0;
                volume = (128 * right_volume()) / 7;
                if (m_ch0.right_enabled()) {
                    buffer_in = ((float)m_ch0.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_ch1.right_enabled()) {
                    buffer_in = ((float)m_ch1.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_wave.right_enabled()) {
                    buffer_in = ((float)m_wave.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                if (m_noise.right_enabled()) {
                    buffer_in = ((float)m_noise.output()) / 100;
                    SDL_MixAudioFormat((Uint8*)&buffer_out, (Uint8*)&buffer_in, AUDIO_F32SYS, sizeof(float), volume);
                }
                m_audio_buffer[m_buffer_fill + 1] = buffer_out;

                m_buffer_fill += 2;
            }

            if (m_buffer_fill >= sample_size) {
                m_buffer_fill = 0;
                while ((SDL_GetQueuedAudioSize(1)) > sample_size * sizeof(float));
                SDL_QueueAudio(1, m_audio_buffer, sample_size*sizeof(float));
            }
            ticks--;
        }
    }

    WaveChannel &Apu::wave() {
        return m_wave;
    }

    SquareChannel &Apu::ch0() {
        return m_ch0;
    }

    SquareChannel &Apu::ch1() {
        return m_ch1;
    }

    NoiseChannel &Apu::noise() {
        return m_noise;
    }

    register_8_t Apu::left_volume() {
        return (m_nr50 & 0b0111'0000u) >> 0x4u;
    }

    register_8_t Apu::right_volume() {
        return (m_nr50 & 0b0000'0111u);
    }
}