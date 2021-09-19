#include <version.h>

#include <iostream>
#include <logging/logging.h>
#include <emulator/dmg.h>
#include <emulator/sdl2.h>

int main(int argc, char *argv[])
{
    emulator::Logger::init(spdlog::level::trace);
    auto log = emulator::Logger::get_logger("main");
    try
    {
        SPDLOG_LOGGER_INFO(&log, "dmg version {}.{}", DMG_VERSION_MAJOR, DMG_VERSION_MINOR);
        auto sdl = new emulator::Sdl2Host();
        emulator::Dmg dmg(false, emulator::boot_room, "", sdl);
        dmg.run();
        return 0;
    }
    catch (std::exception &err)
    {
        log.error(err.what());
        return 1;
    }
}