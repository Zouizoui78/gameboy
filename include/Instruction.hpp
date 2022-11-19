#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

struct Operand {
    std::string name = "";
    bool immediate = true;
};

struct Instruction {
    std::string mnemonic = "";
    uint8_t bytes = 1;
    std::vector<uint8_t> cycles { 1 }; // M-cycles
    std::vector<Operand> operands;
    bool immediate = true;
    int z = -1;
    int n = -1;
    int h = -1;
    int c = -1;

    // Flags :
    //      -1 : untouched
    //       0 : set to 0
    //       1 : set to 1
    //       2 : normal flag effect
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Operand,
    name,
    immediate
)

void to_json(nlohmann::json& j, const Instruction& p);
void from_json(const nlohmann::json& j, Instruction& p);

#endif // INSTRUCTION_HPP