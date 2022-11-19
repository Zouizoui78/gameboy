#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Instruction.hpp"
#include "nlohmann/json.hpp"
#include "tools/utils/Log.hpp"

class CPU {
    public:
    
    CPU();
    ~CPU();
    
    void m_cycle(); // "Machine" cycle, or instruction cycle, 1-3 per instruction

    bool load_instructions(const std::string &path);

    bool load_boot_rom(const std::string &path);
    bool load_rom(const std::string &path);

    void log_boot_rom();
    void log_memory(uint16_t start_offset = 0, uint16_t end_offset = 8192);

    private:

    Instruction fetch();

    std::array<Instruction, 256> _instructions;
    std::array<Instruction, 256> _instructions_prefixed;

    static const uint16_t _boot_rom_size = 0x100; // 256 bytes
    std::array<uint8_t, _boot_rom_size> _boot_rom;

    bool _in_boot_rom = true;

    // CPU variables
    uint16_t _pc = 0x0;
    uint16_t _sp = 0x0;
    std::vector<uint16_t> _stack;

    // Memory map

    // Interrupt Enable Register
    // --------------------------- FFFF
    // Internal RAM
    // --------------------------- FF80
    // Empty but unusable for I/O
    // --------------------------- FF4C
    // I/O ports
    // --------------------------- FF00
    // Empty but unusable for I/O
    // --------------------------- FEA0
    // Sprite Attrib Memory (OAM)
    // --------------------------- FE00
    // Echo of 8kB Internal RAM
    // --------------------------- E000
    // 8kB Internal RAM
    // --------------------------- C000
    // 8kB switchable RAM bank
    // --------------------------- A000
    // 8kB Video RAM
    // --------------------------- 8000 --
    // 16kB switchable ROM bank          |
    // --------------------------- 4000  |= 32kB cartridge
    // 16kB ROM bank #0                  |
    // --------------------------- 0000 --
    static const uint32_t _memory_size = 0x10000; // 65536 bytes
    std::array<uint8_t, _memory_size> _memory;

    static const uint16_t _rom_offset = 0x0;
    static const uint16_t _rom_switchable_offset = 0x4000;
    static const uint16_t _vram_offset = 0x8000;
    static const uint16_t _ram_switchable_offset = 0xa000;
    static const uint16_t _ram_offset = 0xc000;
    static const uint16_t _ram_echo_offset = 0xe000;
    static const uint16_t _sprites_ram_offset = 0xfe00;
    static const uint16_t _io_offset = 0xff00;
    static const uint16_t _ram2_offset = 0xff80;
    static const uint16_t _interrupt_register_offset = 0xffff;
};

#endif // CPU_HPP