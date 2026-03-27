#include <iostream>
#include <print>
#include <random>
#include <sstream>

using ostring = std::ostringstream;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  using LCG = std::linear_congruential_engine<uint32_t, 1103515245, 12345, 2147483648>;
  LCG engine{std::random_device{}()};

  constexpr const char* source = "!@#$ %^&*()_+ {}|:<>?~`-=[]\\;',./{!@#}@#%^^&*()"
                                 ")_+{}|:\"<>?~`-=[]\\;',.<>/!@#$%^&*()_+{}|:<>?~`"
                                 "-=[]\\;',./@!#$%^&\"*()_+|}{:\"><~?`-=[];'\\,./|}"
                                 "{:?><~!@#$%^&*()_+{}|:\"<>?~`\"+-=[]\\;',./";

  const uint8_t MAX_WORD_LEN = 15;
  const uint8_t WORDS = 100;
  constexpr uint8_t const SOURCE_LEN = []() {
    uint8_t i = 0;
    while (source[i] != '\0')
      i++;
    return i;
  }();

  ostring res_buf;
  for (uint8_t n = 0; n < WORDS; n++) {
    uint8_t len = engine() % MAX_WORD_LEN + 1;
    for (uint8_t i = 0; i < len; i++)
      res_buf << source[engine() % SOURCE_LEN];
    res_buf << ' ';
  }
  std::println("{}", res_buf.str());
}
