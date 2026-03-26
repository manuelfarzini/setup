// clang-format off
#include <iostream>
#include <print>
#include <random>
#include <sstream>

using ostring = std::ostringstream;

constexpr const char * lh = "qwertasdfgzxcvb12345!@#$%";
constexpr const char * rh = "yuiophjklnm,./<>?;:'\"\\|}{+_)(*&^=-09876";
using LCG = std::linear_congruential_engine<uint32_t, 1103515245, 12345, 2147483648>;
LCG engine{std::random_device{}()};
const uint8_t MAX_WORD_LEN = 15;
const uint8_t WORDS = 100;
const uint8_t SRC_LEN = []() {
    uint8_t i = 0;
    while (lh[i] != '\0')
        i++;
    return i;
}();

inline void generate_words(char const * src) {
    ostring obuf;
    for (uint8_t n = 0; n < WORDS; n++) {
        uint8_t len =  engine() % MAX_WORD_LEN + 1;
        for (uint8_t i = 0; i < len; i++)
            obuf << src[engine() % SRC_LEN];
        obuf << ' ';
    }
    std::println("{}", obuf.str());
}

int main(int argc, char ** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc != 2 || strlen(argv[1]) != 1 || (argv[1][0] != 'l' && argv[1][0] != 'r')) {
        std::println("Usage:\n\t{} <l/r>", argv[0]); return 1;
    }

    switch (argv[1][0]) {
        case 'l': generate_words(lh); break;
        case 'r': generate_words(rh); break;
    }
}
