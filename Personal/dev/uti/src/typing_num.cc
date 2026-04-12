#include <format>
#include <iostream>
#include <print>
#include <random>
#include <sstream>

using ostring = std::ostringstream;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    using LCG = std::linear_congruential_engine<
        uint32_t, 1664525, 1013904223, std::numeric_limits<uint32_t>::max()>;
    LCG engine{std::random_device{}()};
    ostring res_buf;
    const uint8_t NUMBERS = 100;
    for (int i = 0; i < NUMBERS; i++) {
        res_buf << std::format("{:x} ", engine());
    }
    std::println("{}", res_buf.str());
}
