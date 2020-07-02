#include <emulator/op.h>

namespace emulator {
    clock_t OpCode::execute() {
        return m_executor(m_z80);
    }
}