#include <iostream>
#include <print>
#include <random>
#include <sstream>

using ostring = std::ostringstream;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using LCG =
        std::linear_congruential_engine<uint32_t, 1103515245, 12345, 2147483648>;
    LCG engine{std::random_device{}()};

    constexpr const char* source = "+=-_}{[]()><123456790/|\\abcmnxyvwpijkj"
                                   "+=-_}{[]()></|\\+=-_}{[]()></|\\"
                                   "+=-_}{[]()></|\\+=-_}{[]()></|\\"
                                   "+=-_}{[]()></|\\+=-_}{[]()></|\\";

    const uint8_t k_max_len = 15;
    const uint8_t k_words = 100;
    constexpr uint8_t const k_src_len = []() {
        uint8_t i = 0;
        while (source[i] != '\0') {
            i++;
        }
        return i;
    }();

    ostring res;
    for (uint8_t n = 0; n < k_words; n++) {
        uint8_t len = engine() % k_max_len + 1;
        for (uint8_t i = 0; i < len; i++) {
            res << source[engine() % k_src_len];
        }
        res << ' ';
    }
    std::println("{}", res.str());
}
