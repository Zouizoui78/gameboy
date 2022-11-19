#include "CPU.hpp"

#include "tools/utils/Log.hpp"
#include "tools/utils/Scheduler.hpp"
#include "tools/utils/Stopwatch.hpp"

#include <string>

int main() {
    #ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
    #endif

    CPU cpu;

    if (!cpu.load_instructions("resources/opcodes.json")) {
        exit(1);
    }

    if (cpu.load_boot_rom("resources/DMG_ROM.bin"))
        cpu.log_boot_rom();

    if (cpu.load_rom("resources/Tetris.gb")) {
        cpu.log_memory(0x0100, 0x0103);
        cpu.log_memory(0x0150, 0x0160);
        cpu.log_memory(0x028b, 0x028f);
    }

    cpu.m_cycle();
}