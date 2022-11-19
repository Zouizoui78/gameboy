#include "Instruction.hpp"

#include "tools/utils/Log.hpp"

using json = nlohmann::json;

std::string flag_int_to_str(const std::string &flag_name, int flag_value) {
    if (flag_value == -1) return "-";
    else if (flag_value == 0) return "0";
    else if (flag_value == 1) return "1";
    else return flag_name;
}

int flat_str_to_int(std::string flag_str) {
    if (flag_str == "-") return -1;
    else if (flag_str == "0") return 0;
    else if (flag_str == "1") return 1;
    else return 2;
}

void to_json(nlohmann::json& j, const Instruction& e) {
    std::vector<uint8_t> cycles = e.cycles;
    for (auto &c : cycles) {
        c *= 4;
    }

    j = json {
        { "mnemonic", e.mnemonic },
        { "bytes", e.bytes },
        { "cycles", cycles },
        { "operands", e.operands },
        { "immediate", e.immediate },
        { "flags", {
            { "Z", flag_int_to_str("Z", e.z) },
            { "N", flag_int_to_str("N", e.n) },
            { "H", flag_int_to_str("H", e.h) },
            { "C", flag_int_to_str("C", e.c) }
        }}
    };
}

void from_json(const nlohmann::json& j, Instruction& e) {
    j.at("mnemonic").get_to(e.mnemonic);
    j.at("bytes").get_to(e.bytes);
    j.at("operands").get_to(e.operands);
    j.at("immediate").get_to(e.immediate);

    j.at("cycles").get_to(e.cycles);
    for (auto &c : e.cycles) {
        c /= 4;
    }

    json flags = j.at("flags");
    e.z = flat_str_to_int(flags.at("Z"));
    e.n = flat_str_to_int(flags.at("N"));
    e.h = flat_str_to_int(flags.at("H"));
    e.c = flat_str_to_int(flags.at("C"));
}