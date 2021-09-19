
#include <emulator/common.h>
#include <string>

namespace emulator
{
    void debug_break()
    {
        raise(SIGTRAP);
    }
}