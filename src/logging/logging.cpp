
#include <logging/logging.h>

namespace emulator {
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::s_console_sink;
    spdlog::level::level_enum Logger::s_level = spdlog::level::debug;
}