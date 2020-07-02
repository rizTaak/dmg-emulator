#ifndef EMULATOR_LOGGING_H
#define EMULATOR_LOGGING_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace emulator {
    class Logger {
        static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> s_console_sink;
        static spdlog::level::level_enum s_level;
    public:
        static void init(spdlog::level::level_enum level) {
            s_level = level;
            Logger::s_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            Logger::s_console_sink->set_level(s_level);
        }

        static spdlog::logger get_logger(const std::string &name) {
            spdlog::logger logger(name, {Logger::s_console_sink});
            logger.set_level(s_level);
            return logger;
        }
    };
}

#endif // EMULATOR_LOGGING_H