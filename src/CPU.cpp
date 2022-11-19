#include "CPU.hpp"

#include <filesystem>

#include "fmt/format.h"
#include "spdlog/fmt/bin_to_hex.h"

#include "tools/utils/Files.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

CPU::CPU() {}
CPU::~CPU() {}

void CPU::m_cycle() {
    Instruction inst = fetch();
    SPDLOG_DEBUG("Fetched instruction : {}", json(inst).dump(4));
}

Instruction CPU::fetch() {
    uint8_t opcode = _in_boot_rom ? _boot_rom[_pc] : _memory[_pc];
    SPDLOG_DEBUG("Fetched opcode : 0x{:x}", opcode);
    Instruction inst = _instructions[opcode];
    _pc++;
    return inst;
}

bool CPU::load_instructions(const std::string &path) {
    std::string raw_json = tools::utils::files::read_text_file(path);
    if (raw_json.empty()) {
        return false;
    }

    try {
        json instructions = json::parse(raw_json);
        for (int i = 0 ; i < 256 ; i++) {
            std::string index = fmt::format("0x{:02X}", i);
            Instruction unprefixed(instructions["unprefixed"][index]);
            Instruction cbprefixed(instructions["cbprefixed"][index]);
            _instructions[i] = unprefixed;
            _instructions_prefixed[i] = cbprefixed;
        }
    }
    catch (const nlohmann::detail::parse_error &e) {
        SPDLOG_ERROR("Failed to parse opcode json : {}", e.what());
        return false;
    }

    SPDLOG_INFO(json(_instructions[0x45]).dump(4));
    SPDLOG_INFO(json(_instructions_prefixed[0x45]).dump(4));
    return true;
}

bool CPU::load_boot_rom(const std::string &path) {
    std::vector<uint8_t> boot_rom = tools::utils::files::read_binary_file(path);
    if (boot_rom.size() != _boot_rom_size) {
        return false;
    }

    for (uint i = 0 ; i < _boot_rom_size ; i++) {
        _boot_rom[i] = boot_rom[i];
    }

    return true;
}

bool CPU::load_rom(const std::string &path) {
    std::vector<uint8_t> rom = tools::utils::files::read_binary_file(path);
    if (rom.size() == 0 || rom.size() > _vram_offset) {
        return false;
    }

    for (uint i = 0 ; i < _vram_offset ; i++) {
        _memory[i] = rom[i];
    }

    return true;
}

void CPU::log_boot_rom() {
    SPDLOG_INFO("Boot rom : {}", spdlog::to_hex(_boot_rom.begin(), _boot_rom.end()));
}

void CPU::log_memory(uint16_t start_offset, uint16_t end_offset) {
    SPDLOG_INFO("Memory from 0x{:x} to 0x{:x} : {}", start_offset, end_offset, spdlog::to_hex(_memory.begin() + start_offset, _memory.begin() + end_offset + 1));
}