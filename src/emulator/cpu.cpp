#include <emulator/cpu.h>
#include <spdlog/fmt/fmt.h>
#include <emulator/gpu.h>

namespace emulator {

    Cpu::Cpu(Memory &mem) :
            log{Logger::get_logger("cpu")},
            m_previous_states {},
            m_mem {mem},
            m_gpu {},
            m_interrupt_enable_register {mem.get_ref_byte(mem_registers::interrupt_enable_register)},
            m_interrupt_req_register {mem.get_ref_byte((mem_registers::interrupt_req_register))}{
        add_0x();
        add_1x();
        add_2x();
        add_3x();
        add_4x();
        add_5x();
        add_6x();
        add_7x();
        add_8x();
        add_9x();
        add_ax();
        add_bx();
        add_cx();
        add_dx();
        add_ex();
        add_fx();

        add_cb0x();
        add_cb1x();
        add_cb2x();
        add_cb3x();
        add_cb4x();
        add_cb5x();
        add_cb6x();
        add_cb7x();
        add_cb8x();
        add_cb9x();
        add_cbax();
        add_cbbx();
        add_cbcx();
        add_cbdx();
        add_cbex();
        add_cbfx();
    }

    void Cpu::reset() {
        m_a = m_b = m_c = m_d = m_e = m_f = m_h = m_l = 0;
        m_pc = 0x0000;
        m_sp = 0xffee;
        m_last = m_total = 0;
    }

    void Cpu::fetch_decode_execute() {
        m_ins_count++;
        handle_halt_bug();
        //auto state = get_pretty_state();
        //SPDLOG_LOGGER_INFO(&log, state);
        //add_state(state);
        m_last = 0;
        auto cycles = handle_interrupt();
        //todo: interrupt cycles
        //m_last += cycles;
        //m_total += cycles;
        if(!halted()) {
            halt_bug:
            auto op = fetch();
            if (op != 0xcb) {
                try {
                    if(0x10 == op && !m_master_interrupt && any_interrupts_pending()) {
                        // not handling multi byte inst!
                        enable_halt_bug();
                        goto halt_bug;
                    } else {
                        auto it = m_ops.find(op);
                        if (it == m_ops.end()) {
                            throw std::runtime_error(fmt::format("unknown op code {:#04x}", op));
                        }
                        cycles += it->second.execute();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(fmt::format("error in op code {0:#04x}: {1}", op, e.what()));
                }
            } else {
                try {
                    op = fetch();
                    auto it = m_bc_ops.find(op);
                    if (it == m_bc_ops.end()) {
                        throw std::runtime_error(fmt::format("unknown cb op code {:#04x}", op));
                    }
                    cycles += it->second.execute();
                } catch (std::exception &e) {
                    throw std::runtime_error(
                            fmt::format("error in cb-op code {0:#04x}: {1}", op, e.what()));
                }
            }
        } else {
            cycles += 4;
        }
        m_total += cycles;
        m_last += cycles;
        handle_mi();
    }

    std::string Cpu::get_pretty_state() {
        return fmt::format("state: PC:{0:#06x} OP::{8:#06x} SP:{1:#06x} AF:{2:#06x} "
                           "BC:{3:#06x} DE:{4:#06x} HL:{5:#06x} F:{6:#010b} SL:{7:03}",
                           m_pc, m_sp, m_af, m_bc, m_de, m_hl, m_f, m_mem.read_byte(0xff44), peek_pc());
    }
    void Cpu::add_state(std::string &state) {
        m_previous_states.push_back(state);
        if (m_previous_states.size() > 10)
            m_previous_states.pop_front();
    }

    void Cpu::log_history() {
        for(auto &&state: m_previous_states) {
            SPDLOG_LOGGER_INFO(&log, state);
        }
    }

    void Cpu::stop() {
        m_stopped = true;
    }

    void Cpu::skip_boot() {
        m_mem.write_byte(mem_registers::boot_rom_disabled, 0x00);
        init_for_catridge();
    }

    register_8_t Cpu::fetch() {
        auto op = m_mem.read_byte(m_pc);
        m_pc++;
        return op;
    }

    register_16_t Cpu::peek_pc() {
        auto op = m_mem.read_byte(m_pc);
        if (op == 0xcb) {
            return (static_cast<register_16_t>(op) << 8u) + m_mem.read_byte(m_pc+1);
        } else {
            return static_cast<register_16_t>(op);
        }
    }

    register_16_t Cpu::wfetch() {
        auto op = m_mem.read_word(m_pc);
        m_pc += 2;
        return op;
    }

    void Cpu::wpush(register_16_t value) {
        m_sp -= 2;
        m_mem.write_word(m_sp, value);
    }

    register_16_t Cpu::wpop() {
        register_16_t value = m_mem.read_word(m_sp);
        m_sp += 2;
        return value;
    }

    void Cpu::enable_interrupt() {
        //log.info("enable interrupt req");
        if(!m_master_interrupt)
            m_mi_enable_pending = 2;
    }

    void Cpu::disable_interrupt() {
        //log.info("disable interrupt req");
        //if(m_master_interrupt)
        m_mi_enable_pending = 0;
        m_master_interrupt = false;
    }

    void Cpu::handle_mi() {
        if (m_mi_enable_pending > 0) {
            m_mi_enable_pending--;
            if (m_mi_enable_pending == 0) {
                m_master_interrupt = true;
                //log.info("enabled interrupt");
            }
        }
    }

     clock_t Cpu::handle_interrupt() {
        clock_t ticks {0};
        if (m_master_interrupt) {
            if (vblank_req() && vblank_enabled()) {
                vblank_clear_interrupt();
                ticks += handle_vblank_interrupt();
                unhalt();
                return ticks;
            }

            if (lcdc_req() && lcdc_enabled()) {
                lcdc_clear_interrupt();
                ticks += handle_lcdc_interrupt();
                unhalt();
                return ticks;
            }

            if (timer_req() && timer_enabled()) {
                timer_clear_interrupt();
                ticks += handel_timer_interrupt();
                unhalt();
                return ticks;
            }

            if (joy_pad_req() && joy_pad_enabled()) {
                joy_pad_clear_interrupt();
                ticks += handel_joy_pad_interrupt();
                unhalt();
                return ticks;
            }
            return 0;
        } else {
            if (should_unhalt()) {
                unhalt();
            }
            return 0;
        }
    }

    const clock_t interrupt_m_cycles = 20;
    clock_t Cpu::handle_vblank_interrupt() {
        //log.info("INTERRUPT-vblank");
        in_vblank = true;
        m_master_interrupt = false;
        wpush(m_pc);
        m_pc = 0x0040u;
        return interrupt_m_cycles;
    }

    clock_t Cpu::handle_lcdc_interrupt() {
        //log.info("INTERRUPT-lcdc");
        m_master_interrupt = false;
        wpush(m_pc);
        m_pc = 0x0048u;
        return interrupt_m_cycles;
    }

    clock_t Cpu::handel_timer_interrupt() {
        //log.info("INTERRUPT-timer");
        m_master_interrupt = false;
        wpush(m_pc);
        m_pc = 0x0050u;
        return interrupt_m_cycles;
    }

    clock_t Cpu::handel_joy_pad_interrupt() {
        //log.info("INTERRUPT-joy_pad");
        m_master_interrupt = false;
        wpush(m_pc);
        m_pc = 0x0060u;
        return interrupt_m_cycles;
    }

    void Cpu::handle_halt_bug() {
        if(m_haltbug) {
            m_pc = m_haltbug_pc;
            clear_halt_bug();
        }
    }

    void Cpu::init_for_catridge() {
        m_pc = 0x0100;
        m_af = 0x01b0;
        m_bc = 0x0013;
        m_de = 0x00d8;
        m_hl = 0x014d;
        m_sp = 0xfffe;

        m_mem.write_byte(0xFF00, 0x0F);
        m_mem.write_byte(0xFF05, 0x00);
        m_mem.write_byte(0xFF06, 0x00);
        m_mem.write_byte(0xFF07, 0x00);

        m_mem.write_byte(0xFF10, 0x80);
        m_mem.write_byte(0xFF11, 0xBF);
        m_mem.write_byte(0xFF12, 0xF3);
        m_mem.write_byte(0xFF14, 0xBF);
        m_mem.write_byte(0xFF16, 0x3F);
        m_mem.write_byte(0xFF17, 0x00);
        m_mem.write_byte(0xFF19, 0xBF);
        m_mem.write_byte(0xFF1A, 0x7F);
        m_mem.write_byte(0xFF1B, 0xFF);
        m_mem.write_byte(0xFF1C, 0x9F);
        m_mem.write_byte(0xFF1E, 0xBF);

        m_mem.write_byte(0xFF20, 0xFF);
        m_mem.write_byte(0xFF21, 0x00);
        m_mem.write_byte(0xFF22, 0x00);
        m_mem.write_byte(0xFF23, 0xBF);
        m_mem.write_byte(0xFF24, 0x77);
        m_mem.write_byte(0xFF25, 0xF3);
        m_mem.write_byte(0xFF26, 0xF1);

        m_mem.write_byte(0xFF40, 0x91);
        m_mem.write_byte(0xFF42, 0x00);
        m_mem.write_byte(0xFF43, 0x00);
        m_mem.write_byte(0xFF45, 0x00);
        m_mem.write_byte(0xFF47, 0xFC);
        m_mem.write_byte(0xFF48, 0xFF);
        m_mem.write_byte(0xFF49, 0xFF);
        m_mem.write_byte(0xFF4A, 0x00);
        m_mem.write_byte(0xFF4B, 0x00);

        m_mem.write_byte(0xFFFF, 0x00);
        m_mem.write_byte(0xff50, 0xff);
    }
}