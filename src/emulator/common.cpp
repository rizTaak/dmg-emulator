
#include <emulator/common.h>


namespace  emulator {
    void debug_break() {
        raise(SIGTRAP);
    }
}