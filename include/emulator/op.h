#ifndef EMULATOR_OP_H
#define EMULATOR_OP_H

#include <emulator/common.h>
#include <utility>

namespace emulator {
    class Cpu;

    class OpCode {
        Cpu &m_z80;
        opcode_t m_op;
        clock_t (*m_executor)(Cpu& z80);
    public:
        explicit OpCode(Cpu &z80, opcode_t code, clock_t (*executor)(Cpu& z80)) :
                m_z80{z80},
                m_op{code},
                m_executor {executor} {
        }

        [[nodiscard]] opcode_t op() const {
            return m_op;
        }

        clock_t execute();
    };
}

#endif